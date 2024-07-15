#include "lwip/udp.h"
#include "lwip/sys.h"
#include "string.h"
#include <stdio.h>
#include "uart_device.h"
#include "socket_device.h"
#include "sockets.h"
#include "pcf8574.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "udp_server_item.h"

typedef struct {
    int sockfd;
    struct sockaddr_in server_addr;
    QueueHandle_t client_queue;
} udpServer;

typedef struct {
    int client_sock;
} ClientInfo;

static void send_message_to_client(ClientInfo *client, const char *message) {
    if (client && message) {
        send(client->client_sock, message, strlen(message), 0);
    }
}

void udp_server_task(void *pvParameters) {
    udpServer server;
    char rx_buffer[128];
    char addr_str[128];
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(UDP_SRC_SEVER_PORT);
    server.client_queue = xQueueCreate(10, sizeof(ClientInfo));
    server.sockfd = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (server.sockfd < 0) {
        printf("Unable to create socket: errno %d\n", errno);
        vTaskDelete(NULL);
        return;
    }
    printf("Socket created\n");

    int err = bind(server.sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        printf("Socket unable to bind: errno %d\n", errno);
        close(server.sockfd);
        vTaskDelete(NULL);
        return;
    }
    printf("Socket bound, port %d\n", UDP_SRC_SEVER_PORT);

    err = listen(server.sockfd, 1);
    if (err != 0) {
        printf("Error occurred during listen: errno %d\n", errno);
        close(server.sockfd);
        vTaskDelete(NULL);
        return;
    }
    printf("Socket listening\n");

    while (1) {
        struct sockaddr_in source_addr;
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(server.sockfd, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            printf("Unable to accept connection: errno %d\n", errno);
            break;
        }
        inet_ntoa_r(source_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
        printf("Socket accepted ip address: %s\n", addr_str);

        ClientInfo client_info = {.client_sock = sock};
        xQueueSend(server.client_queue, &client_info, portMAX_DELAY);

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0) {
                printf("Recv failed: errno %d\n", errno);
                break;
            } else if (len == 0) {
                printf("Connection closed\n");
                break;
            } else {
                rx_buffer[len] = 0;
                printf("Received %d bytes: %s\n", len, rx_buffer);
                send(sock, rx_buffer, len, 0);
            }
        }

        close(sock);
    }

    close(server.sockfd);
    vTaskDelete(NULL);
}

void start_udp_server() {
    xTaskCreate(udp_server_task, "udp_server_task", 4096, NULL, 5, NULL);
    
}
