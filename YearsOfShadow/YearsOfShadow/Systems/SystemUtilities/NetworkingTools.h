struct PacketDataHeader {
	static constexpr uint16_t PACKET_MAX_LEN = 65000;
	uint64_t messageID;
	uint16_t packetNum;
	uint8_t padding[6];
	uint8_t data[PACKET_MAX_LEN];
};

struct Packet {
	uint8_t* data;
	uint16_t len;
};

struct PacketPool {
	FlatFlaggedBuffer<Packet, 1000> packets;
};

class NetworkMessage {
	std::string ip;
	uint8_t* msg;
	uint32_t size;
	uint64_t id;
	static uint64_t nextID;
	std::vector<Packet*> packets;
public:
	void init(const char* _ip, uint8_t* _msg, uint32_t _size) {
		ip = _ip;
		msg = _msg;
		size = _size;
		id = nextID;
		nextID++;
		uint16_t totalPackets = _size / PacketDataHeader::PACKET_MAX_LEN;
		uint16_t remainder = _size % PacketDataHeader::PACKET_MAX_LEN;
		for (uint32_t i = 0; i < totalPackets; i++)
			pushNewPacket(&_msg[i * PacketDataHeader::PACKET_MAX_LEN], PacketDataHeader::PACKET_MAX_LEN);
		pushNewPacket(&_msg[totalPackets * PacketDataHeader::PACKET_MAX_LEN], remainder);
	}
	void send() {

	}
	bool isFinished() {

	}
	void clean() {
		ip = "";
		free(msg);
		msg = nullptr;
		size = 0;
	}
private:
	void pushNewPacket(const uint8_t* data, uint16_t size) {

	}
};
uint64_t NetworkMessage::nextID = 0;

class NetworkStream {
	HAL::udp_socket_handle_t conn;
	uint16_t inPort, outPort;
	std::vector<NetworkMessage> messages;
public:
	void init(uint16_t _inPort, uint16_t _outPort) {
		inPort = _inPort;
		outPort = _outPort;
	}
	void push(const char* ip, const uint8_t* data, uint32_t size) {
		uint8_t* msg = (uint8_t*)malloc(size);
		if (msg == nullptr) {
			HAL_ERROR("NetworkStream::send() can't allocate enough space for msg!");
			throw;
		}
		memcpy(msg, data, size);
		NetworkMessage nm;
		nm.init(ip, msg, size);
		messages.push_back(nm);
	}
	void sendAll() {
		uint32_t count = (uint32_t)messages.size();
		for (uint32_t i = 0; i < count; i++) {
			messages[i].send();
		}
	}
};