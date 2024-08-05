#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H


#include "stdint.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "stm32h7xx_hal.h"


#define UART_RX_QUEUE_LEN 100
#define UART_TX_QUEUE_LEN 100
#define QUEUE_LENGTH 20

/***
 * ���󴮿�������ؽṹ
 */
typedef struct UART_Data 
{
    UART_HandleTypeDef *handle;         // Uart Data Ӳ�����
    SemaphoreHandle_t xTxSem;           // Uart Data ��������ź�
    SemaphoreHandle_t xMutex;           // Uart Data ����������ֹ���̲߳���ͬһ�ṹ�����쳣
    QueueHandle_t xRxQueue;             // Uart Data ���ݽ��ն��У���Ž��յ������ݣ��û�����ͨ����ȡ�ζ��л�ȡ��������
    QueueHandle_t xTxQueue;             // Uart Data ���ݷ��Ͷ��У����Ҫ���͵����ݣ��û����Խ�Ҫ���͵����ݲ��뵽�˶�����
    uint8_t rxdata;                     // Uart Data �������ݽ���
    uint8_t lastChar;                   // ��һ���ַ������ڼ�� \r\n
    uint8_t rxBuffer[UART_RX_QUEUE_LEN];// Uart Data �������ݴ洢������ȡ�����ݻ���������
    uint16_t bufferIndex;               // Uart Data rxBuffer ��ǰ����
    uint16_t txLength;                  // Uart Data ��ǰ���нڵ�Ҫ���͵����ݳ���
    uint16_t txCount;                   // Uart Data ��ǰ���нڵ��ѷ��͵����ݳ���
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

UART_DeviceType *New_UART_Device(char *name);


#endif /* __UART_DEVICE_H */
