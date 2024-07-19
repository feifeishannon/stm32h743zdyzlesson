#include "lwip/udp.h"
#include "lwip/sys.h"
#include "string.h"
#include <stdio.h>
#include "uart_device.h"
#include "sockets.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "udp_server_item.h"

typedef struct {
    int sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sockaddr_len;
} udpServer;


static udpServer server;
static char rx_buffer[UDP_RECV_BUF_SIZE];
static char addr_str[128];

int udpServerInit(){
    // 配置服务器地址
    memset((char *)&server.server_addr, 0, sizeof(server.server_addr));
    server.server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server.server_addr.sin_family = AF_INET;   
    server.server_addr.sin_port = htons(UDP_SRC_SEVER_PORT);
    if (socket_device_init() < 0) {
        printf("socket device init err");
        vTaskDelete(NULL);
        return -1;
    };
    // 创建Socket
    server.sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (server.sockfd < 0) {
        printf("Unable to create socket: errno %d\n", errno);
        vTaskDelete(NULL);
        return -1;
    }
    printf("udp Server Socket created\n");

    // 绑定Socket
    int err = bind(server.sockfd, (struct sockaddr *)&server.server_addr, sizeof(server.server_addr));
    if (err < 0) {
        printf("Socket bind failed.\n");
        close(server.sockfd);
        vTaskDelete(NULL);
        return -1;
    }
    printf("UDP server [%s] bind on port %d\n", UDP_DEST_SERVER_IP, UDP_SRC_SEVER_PORT);

    // 初始化客户端地址结构体和长度
    server.sockaddr_len = sizeof(server.client_addr);
    memset(&server.client_addr, 0, sizeof(server.client_addr));

    return  0;
}

void udp_server_task(void *pvParameters) {
    udpServerInit();
    
    while (1) {
        // 接收数据
        int len = recvfrom( server.sockfd, rx_buffer, sizeof(rx_buffer) - 1, 0, 
                            (struct sockaddr *)&server.client_addr, &server.sockaddr_len);
        if (len < 0) {
            printf("Recv failed: errno %d\n", errno);
            break;
        } else if (len == 0) {
            printf("Connection closed\n");
            break;
        } else {
            rx_buffer[len] = 0;
            printf("udp server Received [from %s:%d]: %s\n", 
                UDP_DEST_SERVER_IP, UDP_SRC_SEVER_PORT, rx_buffer);
            const char *reply_msg = "Hello from server";
            int bytes_sent = sendto(server.sockfd, reply_msg, strlen(reply_msg), 0,
                                    (struct sockaddr *)&server.client_addr, server.sockaddr_len);

            if (bytes_sent < 0) {
                printf("Send failed");
            } else {
                printf("Sent response to client %s:%d “%s”\n",
                        inet_ntoa(server.client_addr.sin_addr), ntohs(server.client_addr.sin_port), reply_msg);
            }
        }
    }

    close(server.sockfd);
    vTaskDelete(NULL);
}

void start_udp_server() {
    xTaskCreate(udp_server_task, "udp_server_task", 4096, NULL, 5, NULL);
    
}
