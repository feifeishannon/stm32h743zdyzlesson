#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H


#include "stdint.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "stm32h7xx_hal.h"

/***
 * ���󴮿�������ؽṹ
 */
typedef struct UART_Data 
{
    UART_HandleTypeDef *handle;     // Uart Data Ӳ�����
    SemaphoreHandle_t xTxSem;       // Uart Data ��������ź�
    SemaphoreHandle_t xMutex;       // Uart Data ����������ֹ���̲߳���ͬһ�ṹ�����쳣
    QueueHandle_t xRxQueue;         // Uart Data ���ݽ��ն��У���Ž��յ������ݣ��û�����ͨ����ȡ�ζ��л�ȡ��������
    QueueHandle_t xTxQueue;         // Uart Data ���ݷ��Ͷ��У����Ҫ���͵����ݣ��û����Խ�Ҫ���͵����ݲ��뵽�˶�����
    uint8_t rxdata;                 // Uart Data �������ݴ洢������ÿ���һ�δ��ڶ�ȡ��洢���˱�����
    uint16_t txLength;              // Uart Data ��ǰ���нڵ�Ҫ���͵����ݳ���
    uint16_t txCount;               // Uart Data ��ǰ���нڵ��ѷ��͵����ݳ���
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
