








// TCP �� UDP ��ʼ������

SocketDevice* init_tcp_server(SemaphoreHandle_t xSocketDeviceMutex, uint16_t port) {
    return socket_device_init(xSocketDeviceMutex, SOCK_STREAM, port, NULL);
}

SocketDevice* init_tcp_client(SemaphoreHandle_t xSocketDeviceMutex, const char* ipAddr, uint16_t port) {
    return socket_device_init(xSocketDeviceMutex, SOCK_STREAM, port, ipAddr);
}

SocketDevice* init_udp(SemaphoreHandle_t xSocketDeviceMutex, uint16_t port) {
    return socket_device_init(xSocketDeviceMutex, SOCK_DGRAM, port, NULL);
}

// ʹ��ʾ��
void init_tcp_or_udp_server(){
    SemaphoreHandle_t xSocketDeviceMutex = xSemaphoreCreateMutex();

    // ��ʼ�� TCP ������
    SocketDevice* tcpServer = init_tcp_server(xSocketDeviceMutex, 5001);

    // ��ʼ�� TCP �ͻ���
    SocketDevice* tcpClient = init_tcp_client(xSocketDeviceMutex, "192.168.1.1", 5001);

    // ��ʼ�� UDP socket
    SocketDevice* udpSocket = init_udp(xSocketDeviceMutex, 5010);

    // �ǵ��ڳ������ʱ�ͷ���Դ
    // vSemaphoreDelete(tcpServer->sendLock);
    // vSemaphoreDelete(tcpServer->recvLock);
    // lwip_close(tcpServer->sockfd);
    // free(tcpServer);
    // ���ƴ��� tcpClient �� udpSocket
}
