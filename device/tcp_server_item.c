#include "tcp_client_item.h"
#include "lwip/tcp.h"
#include "sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "queue.h"
#include "semphr.h"
#include "tcp_server_item.h"

typedef struct {
    int sockfd;                         // socket通道
    struct sockaddr_in server_addr;     // server address
    struct sockaddr_in client_addr;     // client address
    socklen_t sockaddr_len;
    QueueHandle_t client_queue;         // client queue handle
} TcpServer;

typedef struct {
    int client_sock;
    struct sockaddr_in clientAddr;
    QueueHandle_t client_data_queue;    // client send data queue handle
} ClientInfo;

static void tcp_server_task(void *arg);
static void tcp_recv_task(void *arg);
static void tcp_send_task(void *arg);

TcpServer tcp_server;
SemaphoreHandle_t printf_xMutex;

static void tcp_recv_task(void *arg)
{
    ClientInfo *clientInfo = (ClientInfo *)arg;
    char buffer[128];
    int bytes_received;
    int should_clean_up = 0;
    static SemaphoreHandle_t cleanupSemaphore = NULL;
    // Initialize the cleanup semaphore if it hasn't been initialized yet
    if (cleanupSemaphore == NULL) {
        cleanupSemaphore = xSemaphoreCreateBinary();
        xSemaphoreGive(cleanupSemaphore); // Start with semaphore available
    }
    if (xSemaphoreTake(printf_xMutex, (TickType_t) 20) == pdTRUE) {
        printf("xTaskCreate tcp_recv_task[%s:%d]\n",
                inet_ntoa(clientInfo->clientAddr.sin_addr), ntohs(clientInfo->clientAddr.sin_port));
        vTaskDelay(pdMS_TO_TICKS(10));
        xSemaphoreGive(printf_xMutex);
    }
    while (1)
    {
        bytes_received = recv(clientInfo->client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            continue;
        } else if (bytes_received == 0) {
            should_clean_up = 1;
            printf("Server disconnected\n");
            break;
        } else{
            buffer[bytes_received] = '\0';
            printf("Received response from client \t%s:%d \t“%s”\n",
                    inet_ntoa(clientInfo->clientAddr.sin_addr), ntohs(clientInfo->clientAddr.sin_port), buffer);
            xQueueSend(clientInfo->client_data_queue, &buffer, portMAX_DELAY);
        }
    }
    // Take the semaphore to ensure exclusive access to cleanup
    if (xSemaphoreTake(cleanupSemaphore, portMAX_DELAY) == pdTRUE) {
        // Close the connection and free the memory
        if (should_clean_up) {
            printf("recv err, close client %s:%d \n",
                    inet_ntoa(clientInfo->clientAddr.sin_addr), ntohs(clientInfo->clientAddr.sin_port));
            close(clientInfo->client_sock);
            free(clientInfo);
        }
        xSemaphoreGive(cleanupSemaphore);
    }

    vTaskDelete(NULL);
}

static void tcp_send_task(void *arg)
{
    ClientInfo *clientInfo = (ClientInfo *)arg;
    // const char *msg = "Hello from TCP server!";
    char msg[128];
    int should_clean_up = 0;
    static SemaphoreHandle_t cleanupSemaphore = NULL;


    // Initialize the cleanup semaphore if it hasn't been initialized yet
    if (cleanupSemaphore == NULL) {
        cleanupSemaphore = xSemaphoreCreateBinary();
        xSemaphoreGive(cleanupSemaphore); // Start with semaphore available
    }
    if (xSemaphoreTake(printf_xMutex, (TickType_t) 20) == pdTRUE) {
        printf("xTaskCreate tcp_send_task[%s:%d]\n",
                inet_ntoa(clientInfo->clientAddr.sin_addr), ntohs(clientInfo->clientAddr.sin_port));
        vTaskDelay(pdMS_TO_TICKS(10));
        xSemaphoreGive(printf_xMutex);
    }
    
    while (1)
    {
        // Wait for a message to send
        if (xQueueReceive(clientInfo->client_data_queue, &msg, portMAX_DELAY) == pdPASS) {
            // Send the message to the server
            if (send(clientInfo->client_sock, msg, strlen(msg), 0) < 0) {
                should_clean_up = 1;
                printf("Failed to send data\n");
                continue;
            }
            printf("send response to client \t%s:%d \t“%s”\n",
                    inet_ntoa(clientInfo->clientAddr.sin_addr), ntohs(clientInfo->clientAddr.sin_port), msg);
            // Free the message buffer
            memset(msg, 0, sizeof(msg));
        }
    }

    // Take the semaphore to ensure exclusive access to cleanup
    if (xSemaphoreTake(cleanupSemaphore, portMAX_DELAY) == pdTRUE) {
        // Close the connection and free the memory
        if (should_clean_up) {
            printf("send err, close client %s:%d \n",
                    inet_ntoa(clientInfo->clientAddr.sin_addr), ntohs(clientInfo->clientAddr.sin_port));
            close(clientInfo->client_sock);
            free(clientInfo);
        }
        xSemaphoreGive(cleanupSemaphore);
    }
    vTaskDelete(NULL);
}

int tcpServerInit(){
    printf_xMutex = xSemaphoreCreateMutex();
    tcp_server.client_queue = xQueueCreate(10, sizeof(ClientInfo));
    if (tcp_server.client_queue == NULL) {
        printf("Failed to create client queue\n");
        return -1;
    }

    if (socket_device_init() < 0) {
        printf("socket device init err\n");
        return -1;
    };
    // Create socket
    tcp_server.sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (tcp_server.sockfd < 0) {
        handle_lwip_error(errno);
        printf("Unable to create tcp_server socket: errno %d\n", errno);
        return -1;
    }
    printf("tcp_server Socket created\n");

    // Bind to the server port
    tcp_server.server_addr.sin_family = AF_INET;
    tcp_server.server_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_server.server_addr.sin_port = htons(TCP_SRC_SEVER_PORT);
    
    if (bind(tcp_server.sockfd, (struct sockaddr *)&tcp_server.server_addr, sizeof(tcp_server.server_addr)) < 0)
    {
        printf("Failed to bind socket\n");
        close(tcp_server.sockfd);
        return -1;
    }
    printf("tcp_server bind: %d\n", TCP_SRC_SEVER_PORT);

    if (listen(tcp_server.sockfd, 5) < 0)
    {
        printf("Failed to listen on socket\n");
        close(tcp_server.sockfd);
        return -1;
    }
    printf("tcp_server listening\n");

    return 0 ;

}

static void tcp_listen_task(void *arg){
    while (1)
    {
        ClientInfo *clientInfo = malloc(sizeof(ClientInfo));
        socklen_t clientLen = sizeof(clientInfo->clientAddr);
        if (clientInfo == NULL) {
            printf("Memory allocation clientInfo failed\n");
            continue;
        }
        clientInfo->client_data_queue = xQueueCreate(10, sizeof(char[128])); // 独立的发送队列
        if (clientInfo->client_data_queue == NULL) {
            printf("Failed to create client data queue\n");
            free(clientInfo);
            continue;
        }
        clientInfo->client_sock = accept(tcp_server.sockfd, (struct sockaddr*)&clientInfo->clientAddr, &clientLen);
        if (clientInfo->client_sock < 0) {
            free(clientInfo);
            printf("Error accepting connection: %d\n", errno);
            continue;
        }

        // prvLockQueue(tcp_server.client_queue);
        // 将客户端信息添加到队列
        // if (xQueueSend(tcp_server.client_queue, clientInfo, portMAX_DELAY) != pdPASS) {
        //     printf("Failed to add client to queue\n");
        //     close(clientInfo->client_sock);
        // } else {
            xTaskCreate(tcp_recv_task, "TCP_Recv_Task", 1024, clientInfo, 5, NULL);
            xTaskCreate(tcp_send_task, "TCP_Send_Task", 1024, clientInfo, 5, NULL);
            // printf("Client added to queue\n");
        // }
        // prvUnlockQueue(tcp_server.client_queue);
    }
}

void start_tcp_server() {
    if(tcpServerInit()>=0) {
        xTaskCreate(tcp_listen_task, "TCP_listen_Task", 1024, NULL, 5, NULL);
    }
}
