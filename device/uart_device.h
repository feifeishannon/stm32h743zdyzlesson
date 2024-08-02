#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H


#include "stdint.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "stm32h7xx_hal.h"

/***
 * 抽象串口数据相关结构
 */
typedef struct UART_Data 
{
    UART_HandleTypeDef *handle;     // Uart Data 硬件句柄
    SemaphoreHandle_t xTxSem;       // Uart Data 发送完成信号
    SemaphoreHandle_t xMutex;       // Uart Data 互斥锁，防止多线程操作同一结构导致异常
    QueueHandle_t xRxQueue;         // Uart Data 数据接收队列，存放接收到的数据，用户可以通过读取次队列获取串口数据
    QueueHandle_t xTxQueue;         // Uart Data 数据发送队列，存放要发送的数据，用户可以将要发送的数据插入到此队列中
    uint8_t rxdata;                 // Uart Data 串口数据存储变量，每完成一次串口读取会存储到此变量中
    uint16_t txLength;              // Uart Data 当前队列节点要发送的数据长度
    uint16_t txCount;               // Uart Data 当前队列节点已发送的数据长度
}UART_DataType;

typedef struct UART_Device
{
    char *name;
    int (*Init) (void);
    void (*Send)(const char *format, ...);
    void (*Sendln)(const char *format, ...);
    int (*Recv)(uint8_t *datas);
    UART_DataType *priv_data;
}UART_DeviceType;

UART_DeviceType *Get_UART_Device(char *name);


#endif /* __UART_DEVICE_H */
