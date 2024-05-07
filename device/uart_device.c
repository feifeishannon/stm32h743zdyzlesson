#include "uart_device.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "stm32h7xx_hal.h"
#include <string.h>

#define UART_RX_QUEUE_LEN 100

extern UART_HandleTypeDef huart1;
static int uart_init(UART_DeviceType *pDev);
static int uart_send(UART_DeviceType *pDev, uint8_t *datas, int len, int timeout_ms);
static int uart_recv(UART_DeviceType *pDev, uint8_t *data, int timeout_ms);

/***
 * 抽象串口对象
 */
typedef struct UART_Data 
{
    UART_HandleTypeDef *handle;
    SemaphoreHandle_t xTxSem;
    QueueHandle_t xRxQueue;
    uint8_t rxdata;
}UART_DataType;

/***
 * 实例化串口数据对象
 */
static UART_DataType g_uart1_data = {
    &huart1,
};

/***
 * 实例化串口对象
 */
UART_DeviceType g_uart1 = {
    "stm32_uart1",
    uart_init,
    uart_send,
    uart_recv,
    &g_uart1_data,
};

UART_DeviceType *g_uart_devs[] = {&g_uart1};

/***
 * 获取串口对象
 */
UART_DeviceType *Get_UART_Device(char *name){
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
        /* 发送数据完成后释放信号量 */
        xSemaphoreGiveFromISR(data->xTxSem, NULL);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    UART_DataType *data;
    if(huart == &huart1) {
        data = g_uart1.priv_data;
        /* 接收到数据后，将数据写入队列中 */
        xQueueSendFromISR(data->xRxQueue, &data->rxdata, NULL);
        HAL_UART_Receive_IT(data->handle, &data->rxdata,1);
    }
}

static int uart_init(UART_DeviceType *pDev){
    UART_DataType *data = pDev->priv_data;
    data-> xTxSem = xSemaphoreCreateBinary();
    data-> xRxQueue = xQueueCreate(UART_RX_QUEUE_LEN, 1);
    HAL_UART_Receive_IT(data->handle, &data->rxdata,1);

    return 0;

}


static int uart_send(UART_DeviceType *pDev, uint8_t *datas, int len, int timeout_ms){
    UART_DataType *uart_data = pDev->priv_data;
    HAL_UART_Transmit_IT(uart_data->handle, datas, len);
    if (pdTRUE == xSemaphoreTake(uart_data->xTxSem, timeout_ms))
        return 0;
    else 
        return -1;
    
}

static int uart_recv(UART_DeviceType *pDev, uint8_t *data, int timeout_ms){
    UART_DataType *uart_data = pDev->priv_data;
    
    /* app读取队列数据 */
    if(pdPASS == xQueueReceive(uart_data->xRxQueue, data, timeout_ms)){
        return 0;
    }else 
        return -1;

}
