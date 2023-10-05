#include <ranges>

namespace NetworkingUtilities {
	constexpr uint16_t PACKET_MAX_LEN = 65000;
	constexpr uint16_t PACKETS_MAX = 200;
	constexpr uint32_t CONNECTIONS_MAX = 10;
	constexpr uint32_t MESSAGES_PER_CONNECTION_MAX = 5;

	class Packet {
		// Though much data is duplicated between packets, it makes things easier/safer.
		struct PacketData {
			uint64_t messageID;
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
			uint8_t padding[3];  //Adjust to keep data buffer 8 byte offset
			uint8_t data[PACKET_MAX_LEN];
		} data;
		uint16_t len;
	public:
		void write(const uint8_t* _data, uint16_t _len,
			uint64_t _messageID, uint16_t _packetNum, uint16_t _packetCount) {
			memcpy(data.data, _data, _len);
			len = _len;
			data.messageID = _messageID;
			data.packetNum = _packetNum;
			data.operationState = PacketData::INITIAL_SEND;
			data.packetCount = _packetCount;
		}
		void send() {

		}
		void confirmToSender() {

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
		uint64_t messageID;
		static uint64_t nextMessageID;
		FlatBuffer<Packet*, PACKETS_MAX> packets;
		PacketPool* packetPoolPtr;
	public:
		//Total data passed must be less than PACKETS_MAX * PACKET_MAX_LEN
		void construct(std::span<const uint8_t> _msg, PacketPool& _packetPool) {
			packetPoolPtr = &_packetPool;
			messageID = nextMessageID++;
			uint16_t totalFullPackets = (uint16_t)(_msg.size() / PACKET_MAX_LEN);
			uint16_t remainder = _msg.size() % PACKET_MAX_LEN;
			if (packets.count) {
				HAL_ERROR("NetworkMessage::construct() called when packets buffer wasn't cleared!");
				throw;
			}
			if (remainder) {
				for (uint32_t i = 0; i < totalFullPackets; i++)
					pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets + 1);
				pushNewPacket(&_msg[totalFullPackets * PACKET_MAX_LEN], remainder, totalFullPackets, totalFullPackets + 1);
			}
			else {
				for (uint32_t i = 0; i < totalFullPackets; i++)
					pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets);
			}
		}
		void deconstruct() {
			uint32_t count = packets.count;
			for (uint32_t i = 0; i < count; i++)
				packetPoolPtr->ret(packets[i]);
			packets.clear();
			packetPoolPtr = nullptr;
		}
		void update() {

		}
		bool isFinished() {

		}
	private:
		void pushNewPacket(const uint8_t* data, uint16_t size, uint16_t packetNum, uint16_t packetCount) {
			Packet* packet = packetPoolPtr->get();
			packet->write(data, size, messageID, packetNum, packetCount);
			packets.push(packet);
		}
	};
	uint64_t NetworkMessage::nextMessageID = 0;

	class NetworkManager {
		PacketPool packetPool;
		//Reused connection handle, no need to clean
		HAL::udp_socket_handle_t conn;
		std::string ourIP;
		//<otherIP, messagesForOtherIP>
		std::unordered_map<std::string, FlatBuffer<NetworkMessage, MESSAGES_PER_CONNECTION_MAX>>
			recvMsgs, sentMsgs;
	public:
		void init(HAL::udp_socket_handle_t _conn) {
			conn = _conn;
			ourIP = HAL::UDP_get_our_ip(conn);
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
			nm.construct(msg, packetPool);
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
			uint16_t outPort;
			while (true) {
				HAL::UDP_get_packet(conn, &buff[0], buff.count, senderIP, outPort);
				if (buff.count == 0)
					continue;
				processPacket(&buff[0], buff.count, senderIP);
			}
		}
	private:
		void processPacket(const uint8_t* data, uint16_t len, std::string_view senderIP) {
				
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