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
} udpClient;

static udpClient client;
static struct sockaddr_in serverAddr;
static int addr_family = AF_INET;
static int ip_protocol = IPPROTO_IP;

// Mutex for socket
static SemaphoreHandle_t xSocketMutex;

static osThreadId_t sendTaskHandle;
static const osThreadAttr_t sendTask_attributes = {
  .name = "sendTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static osThreadId_t recvTaskHandle;
static const osThreadAttr_t recvTask_attributes = {
  .name = "recvTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


// Function prototypes
static void sendTask(void *pvParameters);
static void recvTask(void *pvParameters);
static int udpClientInit(void);


static void handle_lwip_error(err_t err) {
    switch (err) {
        case ERR_OK:
            printf("No error, everything OK.\n");
            break;
        case ERR_MEM:
            printf("Out of memory error.\n");
            break;
        case ERR_BUF:
            printf("Buffer error.\n");
            break;
        case ERR_TIMEOUT:
            printf("Timeout.\n");
            break;
        case ERR_RTE:
            printf("Routing problem.\n");
            break;
        case ERR_INPROGRESS:
            printf("Operation in progress.\n");
            break;
        case ERR_VAL:
            printf("Illegal value.\n");
            break;
        case ERR_WOULDBLOCK:
            printf("Operation would block.\n");
            break;
        case ERR_USE:
            printf("Address in use.\n");
            break;
        case ERR_ALREADY:
            printf("Already connecting.\n");
            break;
        case ERR_ISCONN:
            printf("Conn already established.\n");
            break;
        case ERR_CONN:
            printf("Not connected.\n");
            break;
        case ERR_IF:
            printf("Low-level netif error.\n");
            break;
        case ERR_ABRT:
            printf("Connection aborted.\n");
            break;
        case ERR_RST:
            printf("Connection reset.\n");
            break;
        case ERR_CLSD:
            printf("Connection closed.\n");
            break;
        case ERR_ARG:
            printf("Illegal argument.\n");
            break;
        default:
            printf("Unknown error: %d\n", err);
            break;
    }
}


static int udpClientInit(){
    
    // Create socket
    client.sockfd = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (client.sockfd < 0) {
        printf("Unable to create socket: errno %d\n", errno);
        return -1;
    }
    printf("Socket created, connecting to %s:%d\n", UDP_DEST_SERVER_IP, UDP_DEST_SEVER_PORT);

    // Configure server address
    client.server_addr.sin_addr.s_addr = inet_addr(UDP_DEST_SERVER_IP);
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(UDP_DEST_SEVER_PORT);
    memset(&(client.server_addr.sin_zero), 0, sizeof(client.server_addr.sin_zero));

    // Connect to server
    int err = connect(client.sockfd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr));
    if (err != 0) {
        handle_lwip_error(errno);
        // printf("Socket unable to connect: errno %d\n", errno);
        close(client.sockfd);
        return -1;
    }
    printf("Successfully connected\n");

    // Initialize socket mutex
    xSocketMutex = xSemaphoreCreateMutex();
    if (xSocketMutex == NULL)
    {
        close(client.sockfd);
        handle_lwip_error(errno);
        // Handle error
        return -1;
    }
    return 0 ;
}


// Task for sending data
static void sendTask(void *pvParameters) {
    char sendBuf[UDP_SEND_BUF_SIZE];
    int err = 0;
    while (1) {
        static uint8_t errtimes = 0;
        // Fill send buffer with data
        snprintf(sendBuf, UDP_SEND_BUF_SIZE, "Hello, Server!");
        
        // Lock the socket for sending
        if (xSemaphoreTake(xSocketMutex, portMAX_DELAY) == pdTRUE) {
            err = send(client.sockfd, sendBuf, strlen(sendBuf), 0);
            xSemaphoreGive(xSocketMutex);
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
static void recvTask(void *pvParameters) {
    char recvBuf[UDP_RECV_BUF_SIZE];
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

void start_udp_client() {
    int err = udpClientInit();
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
