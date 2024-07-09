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
struct sockaddr_in serverAddr;
int addr_family = AF_INET;
int ip_protocol = IPPROTO_IP;

char rx_buffer[128];
char tx_buffer[128];

// Mutex for socket
static SemaphoreHandle_t xSocketMutex;

osThreadId_t sendTaskHandle;
const osThreadAttr_t sendTask_attributes = {
  .name = "sendTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t recvTaskHandle;
const osThreadAttr_t recvTask_attributes = {
  .name = "recvTask",
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
        vTaskDelete(NULL);
        return -1;
    }
    printf("Socket created, connecting to %s:%d\n", SERVER_IP, SERVER_PORT);

    // Configure server address
    client.server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(SERVER_PORT);
    memset(&(client.server_addr.sin_zero), 0, sizeof(client.server_addr.sin_zero));

    // Connect to server
    int err = connect(client.sockfd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr));
    if (err != 0) {
        printf("Socket unable to connect: errno %d\n", errno);
        close(client.sockfd);
        vTaskDelete(NULL);
        return -1;
    }
    printf("Successfully connected\n");

    // Initialize socket mutex
    xSocketMutex = xSemaphoreCreateMutex();
    if (xSocketMutex == NULL)
    {
        // Handle error
        return -1;
    }
    return 0 ;
}


// Task for sending data
void sendTask(void *pvParameters) {
    char sendBuf[SEND_BUF_SIZE];
    int err = 0;
    while (1) {
        // Fill send buffer with data
        snprintf(sendBuf, SEND_BUF_SIZE, "Hello, Server!");
        
        // Lock the socket for sending
        if (xSemaphoreTake(xSocketMutex, portMAX_DELAY) == pdTRUE) {
            err = send(client.sockfd, sendBuf, strlen(sendBuf), 0);
            xSemaphoreGive(xSocketMutex);
        }
        if (err != 0) {
            printf("Socket send length: %d\n", err);
        }
        // Delay before next send
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Task for receiving data
void recvTask(void *pvParameters) {
    char recvBuf[RECV_BUF_SIZE];
    int bytesRead;
    while (1) {
        // Lock the socket for receiving
        if (xSemaphoreTake(xSocketMutex, portMAX_DELAY) == pdTRUE) {
            bytesRead = recv(client.sockfd, recvBuf, sizeof(recvBuf) - 1, 0);
            xSemaphoreGive(xSocketMutex);
        }

        if (bytesRead > 0) {
            recvBuf[bytesRead] = '\0';
            printf("Received: %s\n", recvBuf);
        }

        // Delay before next recv
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void start_tcp_client() {
    tcpClientInit();
        // Create Send Task
    sendTaskHandle = osThreadNew(sendTask, NULL, &sendTask_attributes);

    // Create Recv Task
    recvTaskHandle = osThreadNew(recvTask, NULL, &recvTask_attributes);

    // Delete main task
    vTaskDelete(NULL);

    // while (1) {
    //     static int sendtimes = 0;
    //     int len = snprintf(tx_buffer, sizeof(tx_buffer), "Hello server! %d",sendtimes);
    //     int err = send(client.sockfd, tx_buffer, len, 0);
    //     sendtimes++;
    //     if (err < 0) {
    //         printf("Error occurred during sending: errno %d\n", errno);
    //         break;
    //     }

    //     len = recv(client.sockfd, rx_buffer, sizeof(rx_buffer) - 1, 0);
    //     if (len < 0) {
    //         printf("Recv failed: errno %d\n", errno);
    //         break;
    //     } else if (len == 0) {
    //         printf("Connection closed\n");
    //         break;
    //     } else {
    //         rx_buffer[len] = 0;
    //         printf("Received %d bytes: %s\n", len, rx_buffer);
    //     }

    //     vTaskDelay(20 / portTICK_PERIOD_MS);
    // }

    // close(client.sockfd);
    // vTaskDelete(NULL);

}
