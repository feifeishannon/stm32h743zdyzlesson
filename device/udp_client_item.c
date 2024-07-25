#include "udp_client_item.h"
#include "lwip/udp.h"
#include "sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "semphr.h"



typedef struct {
    int sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sockaddr_len;
} udpClient;

static udpClient client;

static osThreadId_t sendTaskHandle;
static const osThreadAttr_t sendTask_attributes = {
  .name = "udpsendTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static osThreadId_t recvTaskHandle;
static const osThreadAttr_t recvTask_attributes = {
  .name = "udprecvTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


// Function prototypes
static void sendTask(void *pvParameters);
static void recvTask(void *pvParameters);
static int udpClientInit(void);

static int udpClientInit(){
    // 配置服务器地址
    client.server_addr.sin_addr.s_addr = inet_addr(UDP_DEST_SERVER_IP);
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(UDP_DEST_SEVER_PORT);
    client.sockaddr_len = sizeof(client.server_addr);
    memset(&(client.server_addr.sin_zero), 0, sizeof(client.server_addr.sin_zero));

    if (socket_device_init() < 0) {
        printf("socket device init err");
        return -1;
    };

    // 创建Socket
    client.sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (client.sockfd < 0) {
        printf("Unable to create socket: errno %d\n", errno);
        return -1;
    }
    printf("UDP client Socket created\n");

    return 0 ;
}


// Task for sending data
static void sendTask(void *pvParameters) {
    char sendBuf[UDP_SEND_BUF_SIZE];
    int bytes_sent  = 0;
    while (1) {
        static uint8_t errtimes = 0;
        // Fill send buffer with data
        snprintf(sendBuf, UDP_SEND_BUF_SIZE, "Hello, Server!");
        
        // Lock the socket for sending
        if (xSemaphoreTake(socketDevice.sendOrRecvLock, portMAX_DELAY) == pdTRUE) {
            bytes_sent  = sendto(client.sockfd, sendBuf, strlen(sendBuf), 0,
                                (struct sockaddr *)&client.server_addr, client.sockaddr_len);
            printf("udp client sendto server[%s:%d] %s\n", 
                    UDP_DEST_SERVER_IP, UDP_DEST_SEVER_PORT, sendBuf);
            xSemaphoreGive(socketDevice.sendOrRecvLock);
        }
        if (bytes_sent  == -1) {
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
static void recvTask(void *pvParameters) {
    char recvBuf[UDP_RECV_BUF_SIZE];
    int bytesRead;
    while (1) {
        // Lock the socket for receiving
        if (xSemaphoreTake(socketDevice.sendOrRecvLock, portMAX_DELAY) == pdTRUE) {
            bytesRead = recvfrom( client.sockfd, recvBuf, sizeof(recvBuf) - 1, 0, 
                                (struct sockaddr *)&client.server_addr, &client.sockaddr_len);
            xSemaphoreGive(socketDevice.sendOrRecvLock);
        }

        if (bytesRead > 0) {
            recvBuf[bytesRead] = '\0';
            printf("udp client Received from[%s:%d] %s\n", 
                    UDP_DEST_SERVER_IP, UDP_DEST_SEVER_PORT, recvBuf);
        }

        // Delay before next recv
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void start_udp_client() {
    if(udpClientInit()>=0) {
        // Create Send Task
        sendTaskHandle = osThreadNew(sendTask, NULL, &sendTask_attributes);

        // Create Recv Task
        recvTaskHandle = osThreadNew(recvTask, NULL, &recvTask_attributes);
    }
}
