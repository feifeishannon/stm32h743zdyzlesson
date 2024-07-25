








// TCP 和 UDP 初始化函数

SocketDevice* init_tcp_server(SemaphoreHandle_t xSocketDeviceMutex, uint16_t port) {
    return socket_device_init(xSocketDeviceMutex, SOCK_STREAM, port, NULL);
}

SocketDevice* init_tcp_client(SemaphoreHandle_t xSocketDeviceMutex, const char* ipAddr, uint16_t port) {
    return socket_device_init(xSocketDeviceMutex, SOCK_STREAM, port, ipAddr);
}

SocketDevice* init_udp(SemaphoreHandle_t xSocketDeviceMutex, uint16_t port) {
    return socket_device_init(xSocketDeviceMutex, SOCK_DGRAM, port, NULL);
}

// 使用示例
void init_tcp_or_udp_server(){
    SemaphoreHandle_t xSocketDeviceMutex = xSemaphoreCreateMutex();

    // 初始化 TCP 服务器
    SocketDevice* tcpServer = init_tcp_server(xSocketDeviceMutex, 5001);

    // 初始化 TCP 客户端
    SocketDevice* tcpClient = init_tcp_client(xSocketDeviceMutex, "192.168.1.1", 5001);

    // 初始化 UDP socket
    SocketDevice* udpSocket = init_udp(xSocketDeviceMutex, 5010);

    // 记得在程序结束时释放资源
    // vSemaphoreDelete(tcpServer->sendLock);
    // vSemaphoreDelete(tcpServer->recvLock);
    // lwip_close(tcpServer->sockfd);
    // free(tcpServer);
    // 类似处理 tcpClient 和 udpSocket
}
