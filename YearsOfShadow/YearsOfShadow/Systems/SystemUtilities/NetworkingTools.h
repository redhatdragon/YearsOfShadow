#include <ranges>
#include <string>
#include <unordered_map>



/*
* TODO: refactor this entire abstraction after situation is felt out.
*/



//TODO: figure out where to put this, hashes string_view
template<class Char, class CharTraits = std::char_traits<Char>>
class string_hash
{
	using is_transparent = void;

	[[nodiscard]] std::size_t operator()(const Char* str) const {
		return std::hash<std::basic_string_view<Char, CharTraits>>{}(str);
	}

	[[nodiscard]] std::size_t operator()(std::basic_string_view<Char, CharTraits> str) const {
		return std::hash<std::basic_string_view<Char, CharTraits>>{}(str);
	}

	template<class Allocator>
	[[nodiscard]] std::size_t operator()(const std::basic_string<Char, CharTraits, Allocator>& str) const {
		return std::hash<std::basic_string_view<Char, CharTraits>>{}(str);
	}
};

std::unordered_map<std::string, int, string_hash<char>, std::equal_to<>> sus{};



namespace NetworkingUtilities {
	constexpr uint16_t PACKET_MAX_LEN = 65000;
	constexpr uint16_t PACKETS_MAX = 6000;  //Shared between all connections/messages
	constexpr uint16_t PACKETS_PER_MESSAGE_MAX = 100;
	constexpr uint32_t CONNECTIONS_MAX = 10;
	constexpr uint32_t MESSAGES_PER_CONNECTION_MAX = 120;

	struct Packet {
		// Though much data is duplicated between packets, it makes things easier/safer.
		uint16_t dataLen;
		uint16_t packetNum;
		uint16_t packetCount;
		enum OPERATION_STATE : uint8_t {
			//Send packet with valid data buffer
			INITIAL_SEND,
			//Get confirmation that sent packet made it
			CONFIRMING_SENT,
			//Let other know we didn't yet receive confirmation, could be useful for debugging
			REQUEST_CONFIRMATION
		} operationState;
		uint8_t padding[1];  //Adjust to keep data buffer 8 byte offset
		char signature[8];  //YoS Pack
		uint64_t messageID;
		uint8_t data[PACKET_MAX_LEN];
		void write(const uint8_t* _data, uint16_t _len,
			uint64_t _messageID, uint16_t _packetNum, uint16_t _packetCount) {
			memcpy(data, _data, _len);
			dataLen = _len;
			memcpy(signature, "YoS Pack", 8);
			messageID = _messageID;
			packetNum = _packetNum;
			operationState = INITIAL_SEND;
			packetCount = _packetCount;
		}
		inline void cpyFrom(const Packet& other) {
			memcpy(this, &other, other.dataLen + getHeaderSize());
		}
		inline bool isSignatureValid() const {
			if (strncmp(signature, "YoS Pack", 8) == 0)
				return true;
			return false;
		}
		inline static uint8_t getHeaderSize() {
			return 8 + 8 + 8;
		}
		inline void sendTo(HAL::udp_socket_handle_t conn, std::string_view ip) {
			HAL::UDP_send_packet(conn, (uint8_t*)this, getHeaderSize() + dataLen, ip.data());
		}
	};

	struct PacketPool {
		Packet packets[PACKETS_MAX];
		FlatBuffer<Packet*, PACKETS_MAX> packetPtrs;
		void setup() {
			for (uint32_t i = 0; i < PACKETS_MAX; i++) {
				packetPtrs[i] = &packets[i];
			}
			packetPtrs.count = PACKETS_MAX;
		}
		Packet* get() {
			packetPtrs.pop();
			Packet* retValue = packetPtrs[packetPtrs.count];
			return retValue;
		}
		void ret(Packet* packet) {
			packetPtrs.push(packet);
		}
	};
	constexpr int sizeOfPacketPool = sizeof(PacketPool);

	class NetworkMessage {
	public:
		//uint64_t messageID;
		//static uint64_t nextMessageID;
		std::array<Packet*, PACKETS_PER_MESSAGE_MAX> packets;
		PacketPool* packetPoolPtr;
		uint64_t tick;  //Tracked for number of update ticks passed

		//Total data passed must be less than PACKETS_MAX * PACKET_MAX_LEN
		bool tryConstructForSend(std::span<const uint8_t> _msg, PacketPool& _packetPool, uint64_t id) {
			//TODO: consider getting rid of try prefix, should work anyway?
			tick = 0;
			packetPoolPtr = &_packetPool;
			for (uint32_t i = 0; i < packets.size(); i++)
				packets[i] = nullptr;
			uint16_t totalFullPackets = (uint16_t)(_msg.size() / PACKET_MAX_LEN);
			uint16_t remainder = _msg.size() % PACKET_MAX_LEN;
			if (remainder) {
				for (uint32_t i = 0; i < totalFullPackets; i++)
					pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets + 1, id);
				pushNewPacket(&_msg[totalFullPackets * PACKET_MAX_LEN], remainder, totalFullPackets, totalFullPackets + 1, id);
			}
			else {
				for (uint32_t i = 0; i < totalFullPackets; i++)
					pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets, id);
			}


			return true;
		}
		bool tryConstructForRecv(const uint8_t* _data, uint16_t len, PacketPool& _packetPool) {
			if (len > PACKET_MAX_LEN) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input len: {} >= PACKET_MAX_LEN: {}",
					len, PACKET_MAX_LEN);
				return false;
			}
			if (len <= Packet::getHeaderSize()) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input len: {} <= packetHeaderSize(): {}\n",
					len, Packet::getHeaderSize());
				return false;
			}
			const Packet* packet = (const Packet*)_data;
			if (packet->isSignatureValid() == false) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input packet's signature is invalid\n");
				char buff[9] = { 0 };
				memcpy(buff, packet->signature, 8);
				HAL_WARN("Expected 'Yos Pack' got '{}'\n", buff);
				return false;
			}
			if (packet->packetNum >= packet->packetCount) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input packet's packetNum >= input packet's packetCount\n");
				return false;
			}
			if (packet->packetCount == 0) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input packet's packets packetCount == 0?!\n");
				return false;
			}
			if (packet->dataLen > len - Packet::getHeaderSize()) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input packet's specified length > actual capacity\n");
				return false;
			}
			if (packet->operationState != Packet::OPERATION_STATE::INITIAL_SEND) {
				HAL_WARN("NetworkMessage::tryConstructForRecv()\n");
				HAL_WARN("input packet's operation state != INITIAL_SEND\n");
				return false;
			}
			tick = 0;
			packetPoolPtr = &_packetPool;
			for (uint32_t i = 0; i < packets.size(); i++)
				packets[i] = nullptr;
			packets[packet->packetNum] = packetPoolPtr->get();
			packets[packet->packetNum]->cpyFrom(*packet);
			return true;
		}
		void deconstruct() {
			for (uint32_t i = 0; i < packets.size(); i++)
				if(packets[i])
					packetPoolPtr->ret(packets[i]);
			packetPoolPtr = nullptr;
		}
		//Return true if packet belongs to this NetworkMessage.
		bool tryUpdate(const uint8_t* _packet, uint16_t len) {
			if(packetPoolPtr == nullptr)
				return false;
			if (len > PACKET_MAX_LEN) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("NetworkMessage::tryUpdate()'s input len: {} >= PACKET_MAX_LEN: {}\n",
					len, PACKET_MAX_LEN);
				return false;
			}
			if (len <= Packet::getHeaderSize()) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("input len: {} <= packetHeaderSize(): {}\n",
					len, Packet::getHeaderSize());
				return false;
			}
			const Packet* inPacketPtr = (const Packet*)_packet;
			if (inPacketPtr->messageID != getMessageID())
				return false;
			if (inPacketPtr->isSignatureValid() == false) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("input packet's signature is invalid\n");
				char buff[9] = { 0 };
				memcpy(buff, inPacketPtr->signature, 8);
				HAL_WARN("Expected 'Yos Pack' got '{}'\n", buff);
				return false;
			}
			if (inPacketPtr->packetNum >= getPacketCount()) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("input packet's packetNum >= packetCount\n");
				return false;
			}
			if (inPacketPtr->packetCount != getPacketCount()) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("input packet's packetCount doesn't match\n");
				return false;
			}
			if (inPacketPtr->operationState != Packet::INITIAL_SEND) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("packet operation: {} != INITIAL_SEND\n", (uint32_t)inPacketPtr->operationState);
				return false;
			}
			Packet* packet = packets[inPacketPtr->packetNum];
			if (packet != nullptr) {
				HAL_WARN("NetworkMessage::tryUpdate()\n");
				HAL_WARN("duplicate packet detected\n");
				return false;
			}
			packet = packets[inPacketPtr->packetNum] = packetPoolPtr->get();
			packet->cpyFrom(*inPacketPtr);
			return true;
		}
		//Auto returns packets
		bool tryPopMsg(std::vector<uint8_t>& out) {
			if (isFinished()) {
				uint32_t count = (uint32_t)packets.size();
				out.reserve(count * PACKET_MAX_LEN);
				for (uint32_t i = 0; i < count; i++) {
					if (packets[i] == nullptr)
						continue;
					uint32_t packLen = packets[i]->dataLen;
					for (uint32_t j = 0; j < packLen; j++)
						out.push_back(packets[i]->data[j]);
					packetPoolPtr->ret(packets[i]);
					packets[i] = nullptr;
				}
				return true;
			}
			return false;
		}
		bool isFinished() {
			for (uint32_t i = 0; i < packets.size(); i++) {
				if (packets[i] == nullptr)
					return false;
				if (i == packets[i]->packetCount - 1)
					return true;
			}
			return true;
		}
		uint64_t getMessageID() {
			Packet* packet = getFirstValidPacket();
			return packet->messageID;
		}
		uint16_t getPacketCount() {
			Packet* packet = getFirstValidPacket();
			return packet->packetCount;
		}
	private:
		void pushNewPacket(const uint8_t* data, uint16_t size, uint16_t packetNum, uint16_t packetCount, uint64_t id) {
			Packet* packet = packetPoolPtr->get();
			packet->write(data, size, id, packetNum, packetCount);
			packets[packetNum] = packet;
		}

		Packet* getFirstValidPacket() {
			for (uint32_t i = 0; i < packets.size(); i++)
				if (packets[i])
					return packets[i];
			HAL_PANIC("NetworkMessage::getFirstPacket() can't find valid packet?!\n");
			return nullptr;
		}
	};
	//uint64_t NetworkMessage::nextMessageID = 0;

	class NetworkManager {
		PacketPool packetPool;
		//Reused connection handle, no need to clean
		HAL::udp_socket_handle_t conn;
		//std::string ourIP;
		uint16_t ourOutPort;
		//<otherIP, messagesForOtherIP>
		std::unordered_map<std::string, FlatBuffer<NetworkMessage, MESSAGES_PER_CONNECTION_MAX>>
			recvMsgs, sentMsgs;
	public:
		void init(HAL::udp_socket_handle_t _conn) {
			conn = _conn;
			//ourIP = HAL::UDP_get_our_ip(conn);
			ourOutPort = HAL::UDP_get_our_port(conn);
			recvMsgs.reserve(CONNECTIONS_MAX);
			sentMsgs.reserve(CONNECTIONS_MAX);

			packetPool.setup();
		}
		void trySendTo(std::string_view ip, const uint8_t* msg, uint32_t len) {
			FlatBuffer<NetworkMessage, MESSAGES_PER_CONNECTION_MAX>* msgsPtr;
			if (sentMsgs.find(ip.data()) == sentMsgs.end()) {
				sentMsgs[ip.data()] = {};
				msgsPtr = &sentMsgs[ip.data()];
				for (uint32_t i = 0; i < msgsPtr->getMax(); i++)
					(*msgsPtr)[i].packetPoolPtr = nullptr;
				msgsPtr->clear();
			} else
				msgsPtr = &sentMsgs[ip.data()];
			if (msgsPtr->isFull()) {
				HAL_WARN("NetworkManager::sendTo() failed to send, overloaded with messages to same ip\n");
				HAL_WARN("IP: {}; MSG_LEN: {}\n", ip, len);
				return;
			}
			NetworkMessage& nm = (*msgsPtr)[msgsPtr->count];
			msgsPtr->count++;
			static uint64_t id = 0;
			nm.tryConstructForSend({msg, len}, packetPool, id);
			id++;
			sendTo(nm, ip);
		}
		void update() {
			constexpr uint64_t maxTicksTillReset = 60 * 2;
			static uint64_t tick = 0;
			for (auto& e : recvMsgs) {
				for (uint32_t i = 0; i < e.second.count; i++) {
					NetworkMessage& nm = e.second[i];
					if (nm.packetPoolPtr) {
						if (tick >= nm.tick + maxTicksTillReset) {
							nm.deconstruct();
							e.second[i] = e.second[e.second.count];
							e.second.pop();
							continue;
						}
						nm.tick++;
					}
				}
			}
			for (auto& e : sentMsgs) {
				for (uint32_t i = 0; i < e.second.count; i++) {
					NetworkMessage& nm = e.second[i];
					if (nm.packetPoolPtr) {
						if (tick >= nm.tick + maxTicksTillReset) {
							nm.deconstruct();
							e.second[i] = e.second[e.second.count];
							e.second.pop();
							continue;
						}
						nm.tick++;
					}
				}
			}
		}

		bool tryPopNextMsg(std::vector<uint8_t>& out) {
			getNewMessages();
			for (auto& e : recvMsgs) {
				for (uint32_t i = 0; i < e.second.count; i++) {
					NetworkMessage& nm = e.second[i];
					if (nm.tryPopMsg(out)) {
						e.second[i] = e.second[e.second.count];
						return true;
					}
				}
			}
			return false;
		}
	private:
		void getNewMessages() {
			static FlatBuffer<uint8_t, 256 * 256> buff;
			std::string senderIP;
			uint16_t outPort;  //TODO: still need to verify port is proper...
			while (true) {
				buff.count = 256 * 256;  //tell max length
				HAL::UDP_get_packet(conn, &buff[0], buff.count, senderIP, outPort);
				if (buff.count == 0)
					break;

				processPacket(&buff[0], buff.count, senderIP, outPort);
			}
		}

		void processPacket(const uint8_t* data, uint16_t len, std::string_view senderIP, uint16_t senderPort) {
			const char* ipStr = senderIP.data();
			if (recvMsgs.find(ipStr) == recvMsgs.end()) {
				recvMsgs[ipStr] = {};
				recvMsgs[ipStr].clear();
			}
			auto& msgBuff = recvMsgs[ipStr];
			uint32_t size = msgBuff.count;
			for (uint32_t i = 0; i < size; i++) {
				if (msgBuff[i].tryUpdate(data, len)) {

					return;
				}
			}
			if (msgBuff.isFull()) {
				HAL_WARN("NetworkManager::processPacket()\n");
				HAL_WARN("msgBuff to ip: {} is already full up to {} elements", ipStr, msgBuff.getMax());
				return;
			}
			NetworkMessage nm;
			nm.tryConstructForRecv(data, len, packetPool);
			msgBuff.push(nm);
			//if(nm.tryConstruct({data, len}, packetPool) == false)
			//	HAL_WARN("Invalid packet came in");
		}

		void sendTo(NetworkMessage& nm, std::string_view ip) {
			uint32_t count = nm.packets[0]->packetCount;
			for (uint32_t i = 0; i < count; i++)
				nm.packets[i]->sendTo(conn, ip);
		}
	};
	constexpr uint64_t sizeofNetworkManager_MB_ish =
		((sizeof(NetworkMessage) * MESSAGES_PER_CONNECTION_MAX * CONNECTIONS_MAX * 2) / 1000000)
		+ sizeof(PacketPool) / 1000000;
}