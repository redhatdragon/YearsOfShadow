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
	constexpr uint16_t PACKETS_MAX = 200;
	constexpr uint32_t CONNECTIONS_MAX = 10;
	constexpr uint32_t MESSAGES_PER_CONNECTION_MAX = 5;

	struct Packet {
		// Though much data is duplicated between packets, it makes things easier/safer.
		uint16_t len;
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
			len = _len;
			memcpy(signature, "YoS Pack", sizeof("YoS Pack"));
			messageID = _messageID;
			packetNum = _packetNum;
			operationState = INITIAL_SEND;
			packetCount = _packetCount;
		}
		inline void cpyFrom(const Packet& other) {
			memcpy(this, &other, other.len + getHeaderSize());
		}
		inline bool isSignatureValid() const {
			if (strcmp(signature, "YoS Pack") == 0)
				return true;
			return false;
		}
		inline static uint8_t getHeaderSize() {
			return 8 + 8 + 8;
		}
	};

	struct PacketPool {
		Packet packets[PACKETS_MAX];
		FlatBuffer<Packet*, PACKETS_MAX> packetPtrs;
		void setup() {
			packetPtrs.clear();
			for (uint32_t i = 0; i < PACKETS_MAX; i++) {
				packetPtrs[i] = &packets[i];
			}
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

	class NetworkMessage {
	public:
		uint64_t messageID;
		static uint64_t nextMessageID;
		FlatBuffer<Packet*, PACKETS_MAX> packets;
		PacketPool* packetPoolPtr;

		//Total data passed must be less than PACKETS_MAX * PACKET_MAX_LEN
		bool tryConstruct(std::span<const uint8_t> _msg, PacketPool& _packetPool) {
			packetPoolPtr = &_packetPool;
			packets.count = 0;
			uint16_t totalFullPackets = (uint16_t)(_msg.size() / PACKET_MAX_LEN);
			uint16_t remainder = _msg.size() % PACKET_MAX_LEN;
			if (packets.count) {
				HAL_ERROR("NetworkMessage::construct() called from non destroyed !");
				throw;
			}
			if (canConstruct(_msg) == false) {
				HAL_WARN("NetworkMessage::construct() failed to construct with input data!");
				return false;
			}
			messageID = nextMessageID++;
			if (remainder) {
				for (uint32_t i = 0; i < totalFullPackets; i++)
					pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets + 1);
				pushNewPacket(&_msg[totalFullPackets * PACKET_MAX_LEN], remainder, totalFullPackets, totalFullPackets + 1);
			}
			else {
				for (uint32_t i = 0; i < totalFullPackets; i++)
					pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets);
			}
			return true;
		}
		void deconstruct() {
			uint32_t count = packets.count;
			for (uint32_t i = 0; i < count; i++)
				packetPoolPtr->ret(packets[i]);
			packets.clear();
			packetPoolPtr = nullptr;
		}
		//Return true if packet belongs to this NetworkMessage.
		bool tryUpdate(const uint8_t* _packet, uint16_t len) {
			const Packet* inPacketPtr = (const Packet*)_packet;
			if (isPacketValid(_packet, len) == false)
				return false;
			if (inPacketPtr->operationState == Packet::INITIAL_SEND) {
				Packet* packet = packets[inPacketPtr->packetNum];
				if (packet == nullptr)
					packet = packets[inPacketPtr->packetNum] = packetPoolPtr->get();
				packet->cpyFrom(*inPacketPtr);
				return true;
			}
			return false;
		}
		//Auto returns packets
		bool tryPopMsg(std::vector<uint8_t>& out) {
			if (isFinished()) {
				uint32_t count = packets.count;
				out.reserve(count * PACKET_MAX_LEN);
				for (uint32_t i = 0; i < count; i++) {
					uint32_t packLen = packets[i]->len;
					for (uint32_t j = 0; j < packLen; j++)
						out.push_back(packets[i]->data[j]);
					packetPoolPtr->ret(packets[i]);
				}
				return true;
			}
			return false;
		}
		bool isFinished() {
			uint32_t count = packets.count;
			for (uint32_t i = 0; i < count; i++)
				if (packets[i] == nullptr)
					return false;
			return true;
		}
	private:
		void pushNewPacket(const uint8_t* data, uint16_t size, uint16_t packetNum, uint16_t packetCount) {
			Packet* packet = packetPoolPtr->get();
			packet->write(data, size, messageID, packetNum, packetCount);
			packets.push(packet);
		}

		bool canConstruct(std::span<const uint8_t> _msg) {
			const Packet* packetPtr = (const Packet*)_msg.data();
			if (_msg.size() <= packetPtr->getHeaderSize()) {
				HAL_WARN("input packet's physical length <= expected header size");
				return false;
			}
			if (packetPtr->isSignatureValid() == false) {
				HAL_WARN("input packet's signature is invalid");
				return false;
			}
			if (packetPtr->packetNum >= packetPtr->packetCount) {
				HAL_WARN("input packet's packetNum >= input packet's packetCount");
				return false;
			}
			if (packetPtr->len > _msg.size() - Packet::getHeaderSize()) {
				HAL_WARN("input packet's specified length > actual capacity");
				return false;
			}
			if (packetPtr->operationState != Packet::OPERATION_STATE::INITIAL_SEND) {
				HAL_WARN("input packet's operation state != INITIAL_SEND");
				return false;
			}
			return true;
		}

		bool isPacketValid(const uint8_t* _packetPtr, uint16_t len) {
			const Packet* packetPtr = (const Packet*)_packetPtr;
			if (packetPtr->messageID != getMessageID())
				return false;
			if (packetPtr->isSignatureValid() == false) {
				HAL_WARN("input packet's signature is invalid");
				return false;
			}
			if (packetPtr->packetNum >= getPacketCount()) {
				HAL_WARN("input packet's packetNum >= packetCount");
				return false;
			}
			if (packetPtr->packetCount != getPacketCount()) {
				HAL_WARN("input packet's packetCount doesn't match");
				return false;
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

		Packet* getFirstValidPacket() {
			uint32_t count = packets.count;
			for (uint32_t i = 0; i < count; i++)
				if (packets[i])
					return packets[i];
			HAL_PANIC("NetworkMessage::getFirstPacket() can't find valid packet?!");
		}
	};
	uint64_t NetworkMessage::nextMessageID = 0;

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
		}
		void sendTo(std::string_view ip, std::span<uint8_t> msg) {
			FlatBuffer<NetworkMessage, MESSAGES_PER_CONNECTION_MAX>* msgsPtr;
			if (sentMsgs.find(ip.data()) == sentMsgs.end()) {
				sentMsgs[ip.data()] = {};
				msgsPtr = &sentMsgs[ip.data()];
				msgsPtr->clear();
			} else
				msgsPtr = &sentMsgs[ip.data()];
			FlatBuffer<NetworkMessage, MESSAGES_PER_CONNECTION_MAX>& msgs = *msgsPtr;
			NetworkMessage& nm = msgs.push({});
			nm.tryConstruct(msg, packetPool);
		}
		//Call once per game tick
		void update() {
			//for (auto& msgs : sentMsgs) {
			//	uint32_t count = msgs.second.count;
			//	for (uint32_t i = 0; i < count; i++) {
			//		NetworkMessage& msg = msgs.second[i];
			//		msg.update();
			//		if (msg.isFinished())
			//			msg.deconstruct();
			//	}
			//}
			//for (auto& msgs : recvMsgs) {
			//	uint32_t count = msgs.second.count;
			//	for (uint32_t i = 0; i < count; i++) {
			//		NetworkMessage& msg = msgs.second[i];
			//		msg.update();
			//	}
			//}

			static FlatBuffer<uint8_t, 256 * 256> buff;
			std::string senderIP;
			uint16_t outPort;  //TODO: still need to verify port is proper...
			while (true) {
				HAL::UDP_get_packet(conn, &buff[0], buff.count, senderIP, outPort);
				if (buff.count == 0)
					break;

				processPacket(&buff[0], buff.count, senderIP, outPort);
			}
		}
		void clear() {
			sentMsgs.clear();
		}

		bool tryPopNextMsg(std::vector<uint8_t>& out) {
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
			NetworkMessage nm;
			if(nm.tryConstruct({data, len}, packetPool) == false)
				HAL_WARN("Invalid packet came in");
		}
	};
	constexpr uint64_t sizeofNetworkManager_MB_ish =
		((sizeof(NetworkMessage) * MESSAGES_PER_CONNECTION_MAX * CONNECTIONS_MAX * 2) / 1000000)
		+ sizeof(PacketPool) / 1000000;

	//Might get renamed/removed soon...
	//Meant to house all messages between one client
	//class NetworkStream {
	//	//HAL::udp_socket_handle_t conn;
	//	std::string outIP;
	//	uint16_t inPort, outPort;
	//	std::vector<NetworkMessage> messages;
	//public:
	//	void init(std::string_view _outIP, uint16_t _inPort, uint16_t _outPort) {
	//		outIP = _outIP;
	//		inPort = _inPort;
	//		outPort = _outPort;
	//		messages.reserve(10);
	//	}
	//	void push(std::string_view ip, const uint8_t* data, uint32_t size) {
	//		//uint8_t* msg = (uint8_t*)malloc(size);
	//		//if (msg == nullptr) {
	//		//	HAL_ERROR("NetworkStream::send() can't allocate enough space for msg!");
	//		//	throw;
	//		//}
	//		//memcpy(msg, data, size);
	//		//messages.push_back(NetworkMessage(ip, {data, size} ));
	//		messages.push_back({});
	//		messages[messages.size() - 1].init(ip, { data, size });
	//	}
	//	void sendAll() {
	//		uint32_t count = (uint32_t)messages.size();
	//		for (uint32_t i = 0; i < count; i++) {
	//			messages[i].send();
	//		}
	//	}
	//	void clear() {
	//		outIP = "";
	//		inPort = outPort = 0;
	//		messages.resize(0);
	//	}
	//	bool consumePacketIfMatching(const uint8_t* packet, std::string_view _outIP) {
	//		if (outIP != _outIP)
	//			return false;
	//		consume(packet);
	//		return true;
	//	}
	//private:
	//	void consume(const uint8_t* packet) {
	//
	//	}
	//};
}