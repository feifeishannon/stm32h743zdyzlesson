#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#define SERVER_IP "192.168.1.99"
#define SERVER_PORT 5001
#define SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define RECV_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define SEND_BUF_SIZE 1024
#define RECV_BUF_SIZE 1024

void start_tcp_client(void);

#endif // TCP_CLIENT_H
