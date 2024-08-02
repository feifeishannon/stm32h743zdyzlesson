/**
  ******************************************************************************
  * @file    uart_device.c
  * @author  
  * @brief   UART �豸�ӿ�.
  *          ��Ҫ freertos ����
  * 
  * 
  ******************************************************************************
    @verbatim
===============================================================================
                        ##### How to use this driver #####
===============================================================================
    [..]
        ����ģʽ����ʵ�ֵ�һ�ӿ�ͨѶ 
    [..]
        ʹ�����̣�
        (#) ʵ����
            (++) UART_DeviceType *UARTdevice;
            (++) UARTdevice = New_UART_Device("stm32_uart1");

        (#) ��ʼ��������ʼ���ڲ��ṹ���������պͷ����߳�
            (++) UARTdevice->Init();

        (#) �������ݡ������� str �ַ���
            (++) UARTdevice->Send(str);

        (#) �������ݡ������յ� txBuffer ��
            (++) UARTdevice->Recv();
            (++) xQueueReceive(UARTdevice->priv_data.xRxQueue, txBuffer, portMAX_DELAY)
            
    @endverbatim
  ******************************************************************************
  */

#include "uart_device.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define UART_RX_QUEUE_LEN 100
#define UART_TX_QUEUE_LEN 100
#define QUEUE_LENGTH 20

extern UART_HandleTypeDef huart1;
static int uart_init(void);
static void uart_send(const char *format, ...);
static void uart_sendln(const char *format, ...);
static int uart_recv(uint8_t *datas);
void uart_tx_task(void *pvParameters);


/***
 * ʵ�����������ݶ���
 * ��ʽ�����ĳ�ԱĬ��ֵΪ0
 */
static UART_DataType g_uart1_data = {
    &huart1,
};

/***
 *  ʵ�������ڶ���
 *  
 *  typedef struct UART_Device{
 *      char *name;
 *      int (*Init)(struct UART_Device *pDev);
 *      int (*Send)(struct UART_Device *pDev, uint8_t *datas, int timeout_ms);
 *      int (*Recv)(struct UART_Device *pDev, uint8_t *data, int timeout_ms);
 *      void *priv_data;
 *  }UART_DeviceType;
 */
static UART_DeviceType g_uart1 = {
    "stm32_uart1",
    uart_init,
    uart_send,
    uart_sendln,
    uart_recv,
    &g_uart1_data,
};

/** 
* �豸�� ���ⲿֻ�ܸ����豸name���Դ��豸���л�ȡ���ڶ���������չʹ��
* @todo: ��ǰ����δʵ�ֶ�̬��չ�豸��Ĺ��ܣ�ֻ���ֶ�����豸
*/
static UART_DeviceType *g_uart_devs[] = {&g_uart1};

/***
 * �������ڶ���
 * �����豸����ƥ�䵽name���豸
 */
UART_DeviceType *New_UART_Device(char *name){
    int i=0;
    for(i = 0 ; i < sizeof(g_uart_devs)/sizeof(g_uart_devs[0]) ; i++){
        if(0 == strcmp(name, g_uart_devs[i]->name))
            return g_uart_devs[i];
    }
    return NULL;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    UART_DataType *data;
    if(huart == &huart1) {
        data = g_uart1.priv_data;
        data->txCount++;
        if(data->txCount>=data->txLength){
            /* ����������ɺ��ͷ��ź��� */
            data->txCount=0;
            data->txLength=0;
            xSemaphoreGiveFromISR(data->xTxSem, NULL);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    UART_DataType *data;
    if(huart == &huart1) {
        data = g_uart1.priv_data;
        /* ���յ����ݺ󣬽�����д������� */
        xQueueSendFromISR(data->xRxQueue, &data->rxdata, NULL);
        HAL_UART_Receive_IT(data->handle, &data->rxdata,1);
    }
}

static int uart_init(){
    UART_DataType *data;
    data = g_uart1.priv_data;
    data->xMutex = xSemaphoreCreateBinary();
    data->xTxSem = xSemaphoreCreateBinary();
    data->xRxQueue = xQueueCreate(UART_RX_QUEUE_LEN, 1);
    data->xTxQueue = xQueueCreate(QUEUE_LENGTH, sizeof(char) * UART_TX_QUEUE_LEN);
    // ��ʼʱ�����ź���
    xSemaphoreGive(data->xTxSem);
    xTaskCreate(uart_tx_task, "UART_TX_Task", 512, (void *)&g_uart1, tskIDLE_PRIORITY + 1, NULL);
    HAL_UART_Receive_IT(data->handle, &data->rxdata,1);
    return 0;
}

static void uart_send(const char *format, ...){
    UART_DataType *data;
    data = g_uart1.priv_data;
    char buffer[UART_TX_QUEUE_LEN];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, UART_TX_QUEUE_LEN, format, args);
    va_end(args);
    xQueueSend(data->xTxQueue, buffer, portMAX_DELAY);
    
}

static void uart_sendln(const char *format, ...){
    UART_DataType *data;
    data = g_uart1.priv_data;
    char buffer[UART_TX_QUEUE_LEN];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, UART_TX_QUEUE_LEN, format, args);
    va_end(args);
    if (len < 0) {
        // Error occurred
        return;
    } else if (len == 0) {
        // No characters were written
        return;
    } else if (len < 99) {
        // 1 to 99 characters were written
        buffer[len] = '\n';  // Append newline at the end
        buffer[len + 1] = '\0';  // Null-terminate the string
    } else {
        // Exactly 100 characters were written
        buffer[99] = '\n';  // Replace the last character with newline
    }
    xQueueSend(data->xTxQueue, buffer, portMAX_DELAY);
}

static int uart_recv(uint8_t *datas){
    UART_DataType *data;
    data = g_uart1.priv_data;
    /* app��ȡ�������� */
    if(pdPASS == xQueueReceive(data->xRxQueue, datas, portMAX_DELAY)){
        return 0;
    }else 
        return -1;
}

/**
 * @brief  ���Ӵ��ڷ��Ͷ��У�������������������ȡ������
 * @note   
 * @param  *pvParameters: �����������Ԥ�贫�봮�ڽӿ�
 * @retval None
 */
void uart_tx_task(void *pvParameters) {
    UART_DataType *data;
    data = g_uart1.priv_data;
    char txBuffer[UART_TX_QUEUE_LEN];
    for (;;) {
        // �ȴ��ź����ͷ�
        if (xSemaphoreTake(data->xTxSem, portMAX_DELAY) == pdTRUE) {
            if (xQueueReceive(data->xTxQueue, txBuffer, portMAX_DELAY) == pdTRUE) {
            // �ӷ��Ͷ����ж�ȡ����
                data->txLength = strlen(txBuffer);
                data->txCount = 0;
                // ʵ�ʷ�������
                HAL_UART_Transmit_IT(data->handle, (uint8_t*)txBuffer, strlen(txBuffer));
            }
        }
    }
}
