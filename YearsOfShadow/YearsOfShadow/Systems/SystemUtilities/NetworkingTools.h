#include <ranges>

constexpr uint16_t PACKET_MAX_LEN = 65000;

class Packet {
	struct PacketData {
		uint64_t messageID;
		uint16_t packetNum;
		//uint8_t toClient;  //0 == true, else toServer
		uint8_t confirmedSent;
		uint16_t packetCount;
		uint8_t padding[2];
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
		//data.toClient = _toClient;
		data.confirmedSent = false;
		data.packetCount = _packetCount;
	}
	void send() {

	}
	void confirmToSender() {

	}
	bool isSent() {
		return (bool)data.confirmedSent;
	}
};

struct PacketPool {
	static constexpr uint32_t max_packets = 10000;
	Packet packets[max_packets];
	FlatBuffer<Packet*, max_packets> packetPtrs;
	PacketPool() {
		packetPtrs.clear();
		for (uint32_t i = 0; i < max_packets; i++) {
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
} packetPool = PacketPool();

class NetworkMessage {
	std::string ip;
	uint64_t messageID;
	static uint64_t nextMessageID;
	FlatBuffer<Packet*, PacketPool::max_packets> packets;
public:
	//NetworkMessage(std::string_view _ip, std::span<const uint8_t> _msg, uint8_t _toClient) {
	//	init()
	//}
	void init(std::string_view _ip, std::span<const uint8_t> _msg) {
		ip = _ip; messageID = nextMessageID++;
		uint16_t totalFullPackets = (uint16_t)(_msg.size() / PACKET_MAX_LEN);
		uint16_t remainder = _msg.size() % PACKET_MAX_LEN;
		if (remainder) {
			for (uint32_t i = 0; i < totalFullPackets; i++)
				pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets+1);
			pushNewPacket(&_msg[totalFullPackets * PACKET_MAX_LEN], remainder, totalFullPackets, totalFullPackets+1);
		} else {
			for (uint32_t i = 0; i < totalFullPackets; i++)
				pushNewPacket(&_msg[i * PACKET_MAX_LEN], PACKET_MAX_LEN, i, totalFullPackets);
		}
	}
	void send() {

	}
	bool isFinished() {

	}
private:
	void pushNewPacket(const uint8_t* data, uint16_t size, uint16_t packetNum, uint16_t packetCount) {
		Packet* packet = packetPool.get();
		packet->write(data, size, messageID, packetNum, packetCount);
		packets.push(packet);
	}
};
uint64_t NetworkMessage::nextMessageID = 0;

class NetworkStream {
	HAL::udp_socket_handle_t conn;
	uint16_t inPort, outPort;
	std::vector<NetworkMessage> messages;
public:
	void init(uint16_t _inPort, uint16_t _outPort) {
		inPort = _inPort;
		outPort = _outPort;
		messages.reserve(10);
	}
	void push(std::string_view ip, const uint8_t* data, uint32_t size) {
		//uint8_t* msg = (uint8_t*)malloc(size);
		//if (msg == nullptr) {
		//	HAL_ERROR("NetworkStream::send() can't allocate enough space for msg!");
		//	throw;
		//}
		//memcpy(msg, data, size);
		//messages.push_back(NetworkMessage(ip, {data, size} ));
		messages.push_back({});
		messages[messages.size() - 1].init(ip, { data, size });
	}
	void sendAll() {
		uint32_t count = (uint32_t)messages.size();
		for (uint32_t i = 0; i < count; i++) {
			messages[i].send();
		}
	}
	void clear() {
		conn = HAL::invalid_udp_socket_handle;
		inPort = outPort = 0;
		messages.resize(0);
	}
};