#include "tcp_client_item.h"
#include "lwip/tcp.h"
#include "sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "semphr.h"



typedef struct {
    int sockfd;
    struct sockaddr_in server_addr;
} TcpClient;

TcpClient client;
static struct sockaddr_in serverAddr;
int addr_family = AF_INET;
int ip_protocol = IPPROTO_IP;



osThreadId_t sendTaskHandle;
const osThreadAttr_t sendTask_attributes = {
  .name = "tcpsendTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t recvTaskHandle;
const osThreadAttr_t recvTask_attributes = {
  .name = "tcprecvTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


// Function prototypes
void sendTask(void *pvParameters);
void recvTask(void *pvParameters);
int tcpClientInit(void);


int tcpClientInit(){
    
    // Create socket
    client.sockfd = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (client.sockfd < 0) {
        printf("Unable to create socket: errno %d\n", errno);
        return -1;
    }
    printf("TCP Client Socket created, connecting to %s:%d\n", TCP_DEST_SERVER_IP, TCP_DEST_SEVER_PORT);

    // Configure server address
    client.server_addr.sin_addr.s_addr = inet_addr(TCP_DEST_SERVER_IP);
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(TCP_DEST_SEVER_PORT);
    memset(&(client.server_addr.sin_zero), 0, sizeof(client.server_addr.sin_zero));

    // Connect to server
    int err = connect(client.sockfd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr));
    if (err != 0) {
        handle_lwip_error(errno);
        // printf("Socket unable to connect: errno %d\n", errno);
        close(client.sockfd);
        return -1;
    }
    printf("TCP Client Socket Successfully connected\n");

    return 0 ;
}


// Task for sending data
void sendTask(void *pvParameters) {
    char sendBuf[TCP_SEND_BUF_SIZE];
    int err = 0;
    while (1) {
        static uint8_t errtimes = 0;
        // Fill send buffer with data
        snprintf(sendBuf, TCP_SEND_BUF_SIZE, "Hello, Server!");
        
        // Lock the socket for sending
        if (xSemaphoreTake(socketDevice.sendOrRecvLock, portMAX_DELAY) == pdTRUE) {
            err = send(client.sockfd, sendBuf, strlen(sendBuf), 0);
            xSemaphoreGive(socketDevice.sendOrRecvLock);
        }
        if (err == -1) {
            handle_lwip_error(errno);
            // printf("Socket send length: %d\n", err);
            if(errtimes++>=10){
                errtimes = 0;
                close(client.sockfd);
                break;
            }
        }
        // Delay before next send
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}

// Task for receiving data
void recvTask(void *pvParameters) {
    char recvBuf[TCP_RECV_BUF_SIZE];
    int bytesRead;
    while (1) {
        // Lock the socket for receiving
        if (xSemaphoreTake(socketDevice.sendOrRecvLock, portMAX_DELAY) == pdTRUE) {
            bytesRead = recv(client.sockfd, recvBuf, sizeof(recvBuf) - 1, 0);
            xSemaphoreGive(socketDevice.sendOrRecvLock);
        }

        if (bytesRead > 0) {
            recvBuf[bytesRead] = '\0';
            printf("tcp client[%s:%d] Received: %s\n", 
                    TCP_DEST_SERVER_IP, TCP_DEST_SEVER_PORT, recvBuf);
        }

        // Delay before next recv
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void start_tcp_client() {
    if(tcpClientInit()>=0) {
        // Create Send Task
        sendTaskHandle = osThreadNew(sendTask, NULL, &sendTask_attributes);

        // Create Recv Task
        recvTaskHandle = osThreadNew(recvTask, NULL, &recvTask_attributes);
    }
    // Delete main task
    // vTaskDelete(NULL);

}
