#include "tcp_client_item.h"
#include "lwip/tcp.h"
#include "sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

typedef struct {
    int sockfd;
    struct sockaddr_in server_addr;
} TcpClient;

// uint8_t tcp_client_init
void start_tcp_client() {
    TcpClient client;

    char rx_buffer[128];
    char tx_buffer[128];
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    client.sockfd = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (client.sockfd < 0) {
        printf("Unable to create socket: errno %d\n", errno);
        vTaskDelete(NULL);
        return;
    }
    printf("Socket created, connecting to %s:%d\n", SERVER_IP, SERVER_PORT);

    client.server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    client.server_addr.sin_family = AF_INET;
    client.server_addr.sin_port = htons(SERVER_PORT);
    memset(&(client.server_addr.sin_zero), 0, sizeof(client.server_addr.sin_zero));

    int err = connect(client.sockfd, (struct sockaddr *)&client.server_addr, sizeof(client.server_addr));
    if (err != 0) {
        printf("Socket unable to connect: errno %d\n", errno);
        close(client.sockfd);
        vTaskDelete(NULL);
        return;
    }
    printf("Successfully connected\n");

    while (1) {
        static sendtimes = 0;
        int len = snprintf(tx_buffer, sizeof(tx_buffer), "Hello server! %d",sendtimes);
        int err = send(client.sockfd, tx_buffer, len, 0);
        sendtimes++;
        if (err < 0) {
            printf("Error occurred during sending: errno %d\n", errno);
            break;
        }

        len = recv(client.sockfd, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0) {
            printf("Recv failed: errno %d\n", errno);
            break;
        } else if (len == 0) {
            printf("Connection closed\n");
            break;
        } else {
            rx_buffer[len] = 0;
            printf("Received %d bytes: %s\n", len, rx_buffer);
        }

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }

    close(client.sockfd);
    vTaskDelete(NULL);

}
