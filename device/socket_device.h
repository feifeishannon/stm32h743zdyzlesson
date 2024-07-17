#ifndef __SOCKET_DEVICE_H
#define __SOCKET_DEVICE_H


#include "stdint.h"
#include "lwip/apps/lwiperf.h"

#include "lwip/tcp.h"
#include "lwip/sys.h"
#include "string.h"
#include <stdio.h>
#include "uart_device.h"
#include "sockets.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

/**
 * @brief  lwip各端口默认分配
 * @note   
 * @retval None
 */

#define TCP_DEST_SERVER_IP      "192.168.1.99"
#define TCP_SRC_SEVER_PORT      5001    
#define TCP_DEST_SEVER_PORT     5003
#define TCP_SEND_BUF_SIZE       1024
#define TCP_RECV_BUF_SIZE       1024
#define TCP_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define TCP_RECV_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#define UDP_DEST_SERVER_IP      "192.168.1.99"
#define UDP_SRC_SEVER_PORT      5011    
#define UDP_DEST_SEVER_PORT     5013
#define UDP_SEND_BUF_SIZE       1024
#define UDP_RECV_BUF_SIZE       1024
#define UDP_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define UDP_RECV_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

typedef struct Socekt_Device
{
    char *name;
    int (*Init)(struct Socekt_Device *pDev);
    int (*Send)(struct Socekt_Device *pDev, uint8_t *datas, int timeout_ms);
    int (*Recv)(struct Socekt_Device *pDev, uint8_t *data, int timeout_ms);
    void *priv_data;
}Socekt_DeviceType;

extern const u8_t lwiperf_txbuf_const[1600];
extern SemaphoreHandle_t xSocketMutex;

Socekt_DeviceType *Get_Socekt_Device(char *name);


#endif /* __SOCKET_DEVICE_H */
