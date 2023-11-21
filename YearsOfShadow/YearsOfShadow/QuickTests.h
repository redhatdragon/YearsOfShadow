#pragma once

void netTest() {
    std::string inChar;
    std::cout << "Enter y if running client" << std::endl;
    std::cin >> inChar;
    HAL::udp_socket_handle_t conn;
    std::string sendStr = "";
    if (inChar == "y") {
        conn = HAL::UDP_open("127.0.0.1", 8122, 8123);
        sendStr = "I'MA FIRIN MA LAZAR!";
    }
    else {
        conn = HAL::UDP_open("127.0.0.1", 8123, 8122);
        sendStr = "WHY DO YOU CUM!";
    }

    static uint8_t buff[100000] = { 0 };
    uint32_t len = 100000;
    std::string outIP;
    uint16_t outPort;
    //HAL::UDP_send_packet(conn, (const uint8_t*)sendStr.c_str(), (uint16_t)sendStr.size());
    while (true) {
        while (true) {
            len = 100000;
            HAL::UDP_get_packet(conn, buff, len, outIP, outPort);
            if (len == 0) break;
            HAL_LOG("received: {}\n", (const char*)buff);
            HAL_LOG("len: {}\n", len);
        }
        HAL_LOG("Sending...\n");
        HAL::UDP_send_packet(conn, (const uint8_t*)sendStr.c_str(), (uint16_t)sendStr.size(), "127.0.0.1");
        Sleep(1000);
    }
    HAL::UDP_close(conn);
}

void netManagerTest() {
    std::string inChar;
    std::cout << "Enter y if running client" << std::endl;
    std::cin >> inChar;
    HAL::udp_socket_handle_t conn;
    std::string sendStr = "";
    if (inChar == "y") {
        conn = HAL::UDP_open("127.0.0.1", 8122, 8123);
        sendStr = "I'MA FIRIN MA LAZAR!";
    }
    else {
        conn = HAL::UDP_open("127.0.0.1", 8123, 8122);
        sendStr = "WHY DO YOU CUM!";
    }

    static NetworkingUtilities::NetworkManager nm;
    nm.init(conn);
    nm.trySendTo("127.0.0.1", (const uint8_t*)sendStr.c_str(), (uint32_t)sendStr.size());

    while (true) {
        std::vector<uint8_t> buff;
        nm.update();
        nm.tryPopNextMsg(buff);
        nm.trySendTo("127.0.0.1", (const uint8_t*)sendStr.c_str(), (uint32_t)sendStr.size());
        if (buff.size()) {
            std::string str;
            str.reserve(buff.size());
            for (uint32_t i = 0; i < buff.size(); i++)
                str.push_back(buff[i]);
            HAL_LOG(str);
        }
        Sleep(1000);
    }
}