static_assert(sizeof(HAL::udp_socket_handle_t) == sizeof(SOCKET),
    "sizeof udp_socket_handle_t != sizeof SOCKET!");

WSADATA wsa;
struct UDP_Connection {
    SOCKET soc;
    uint16_t inPort, outPort;
    void init(SOCKET _soc, uint16_t _inPort, uint16_t _outPort) {
        soc = _soc;
        inPort = _inPort;
        outPort = _outPort;
    }
};

void _network_init() {
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::string errStr = "Error: failed to initialize winsoc, Code: "; errStr += WSAGetLastError();
        HAL::hal_error(errStr);
        throw;
    }
}

HAL::udp_socket_handle_t HAL::UDP_open(const char* ip, uint16_t inPort, uint16_t outPort)
{
    SOCKET inSoc;
    UDP_Connection* conn = (UDP_Connection*)malloc(sizeof(UDP_Connection));
    if ((inSoc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        std::string errStr = "Error: Could not create socket: "; errStr += WSAGetLastError();
        HAL::hal_error(errStr);
        throw;
    }

    unsigned long ul = 1;
    int err;
    err = ioctlsocket(inSoc, FIONBIO, &ul);
    if (err == SOCKET_ERROR) {
        std::string errStr = "Error: Could not ioctlsocket with FIONBIO: "; errStr += WSAGetLastError();
        HAL::hal_error(errStr);
        throw;
    }
    ul = 1;

    int optval = 1;
    setsockopt(inSoc, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof optval);
    //optval = 1;
    //setsockopt(outSoc, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof optval);
    //optval = 1;
    //setsockopt(inSoc, SOL_SOCKET, SO_BROADCAST, (const char*)&optval, sizeof optval);
    //optval = 1;
    //setsockopt(outSoc, SOL_SOCKET, SO_BROADCAST, (const char*)&optval, sizeof optval);

    sockaddr_in local_socaddr = {}, dest_socaddr = {};

    local_socaddr.sin_family = AF_INET;
    //local_socaddr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, ip, &local_socaddr.sin_addr.s_addr);
    local_socaddr.sin_port = htons(inPort);

    if (bind(inSoc, (struct sockaddr*)&local_socaddr, sizeof(local_socaddr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        throw;
    }

    conn->init(inSoc, inPort, outPort);
    udp_socket_handle_t retValue = (udp_socket_handle_t)(uintptr_t)conn;
    return retValue;
}
void HAL::UDP_close(HAL::udp_socket_handle_t _conn)
{
    UDP_Connection* conn = (UDP_Connection*)_conn;
}
void HAL::UDP_send_packet(HAL::udp_socket_handle_t _conn, const uint8_t* data, uint16_t len, const char* ip, uint16_t port)
{
    UDP_Connection* conn = (UDP_Connection*)_conn;
    SOCKET outSoc = conn->soc;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);
    if(port != 0)
        addr.sin_port = htons(port);
    else
        addr.sin_port = htons(conn->outPort);

    if (sendto(outSoc, (const char*)data, len, 0, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("send failed with error %d\n", WSAGetLastError());
        return; 
    }
}
void HAL::UDP_get_packet(HAL::udp_socket_handle_t _conn, uint8_t* outData, uint32_t& outLen,
    uint32_t& outIP, uint16_t& outPort)
{
    UDP_Connection* conn = (UDP_Connection*)_conn;
    SOCKET inSoc = conn->soc;

    sockaddr addr;
    int addrLen = sizeof(addr);
    outLen = recvfrom(inSoc, (char*)outData, outLen, 0 /* no flags*/, (SOCKADDR*)&addr, &addrLen);
    if (outLen == SOCKET_ERROR) {
        outLen = 0;
        outData[0] = '\0';
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            return;
        }
        if (err == 10054) {  //Dumb windows error...
            return;
        }
        printf("recvfrom failed with error %d\n", err);
        return;
    }
    //serverBuf[bytes_received] = '\0';
    outData[outLen] = '\0';
}