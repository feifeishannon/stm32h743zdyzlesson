/**
  ******************************************************************************
  * @file    uart_device.c
  * @author  
  * @brief   UART 设备接口.
  *          需要 freertos 环境
  * 
  * 
  ******************************************************************************
    @verbatim
===============================================================================
                        ##### How to use this driver #####
===============================================================================
    [..]
        单例模式，仅实现单一接口通讯 
    [..]
        使用例程：
        (#) 实例化
            (++) UART_DeviceType *UARTdevice;
            (++) UARTdevice = New_UART_Device("stm32_uart1");

        (#) 初始化——初始化内部结构，创建接收和发送线程
            (++) UARTdevice->Init();

        (#) 发送数据——发送 str 字符串
            (++) UARTdevice->Send(str);

        (#) 接收数据——接收到 txBuffer 中
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
 * 实例化串口数据对象
 * 隐式声明的成员默认值为0
 */
static UART_DataType g_uart1_data = {
    &huart1,
};

/***
 *  实例化串口对象
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
* 设备组 ，外部只能根据设备name属性从设备组中获取串口对象，留作扩展使用
* @todo: 当前代码未实现动态扩展设备组的功能，只能手动添加设备
*/
static UART_DeviceType *g_uart_devs[] = {&g_uart1};

/***
 * 创建串口对象
 * 返回设备组中匹配到name的设备
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
            /* 发送数据完成后释放信号量 */
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
        /* 接收到数据后，将数据写入队列中 */
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
    // 初始时给予信号量
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
    /* app读取队列数据 */
    if(pdPASS == xQueueReceive(data->xRxQueue, datas, portMAX_DELAY)){
        return 0;
    }else 
        return -1;
}

/**
 * @brief  监视串口发送队列，如果队列中有数据则读取并发送
 * @note   
 * @param  *pvParameters: 任务传入参数，预设传入串口接口
 * @retval None
 */
void uart_tx_task(void *pvParameters) {
    UART_DataType *data;
    data = g_uart1.priv_data;
    char txBuffer[UART_TX_QUEUE_LEN];
    for (;;) {
        // 等待信号量释放
        if (xSemaphoreTake(data->xTxSem, portMAX_DELAY) == pdTRUE) {
            if (xQueueReceive(data->xTxQueue, txBuffer, portMAX_DELAY) == pdTRUE) {
            // 从发送队列中读取数据
                data->txLength = strlen(txBuffer);
                data->txCount = 0;
                // 实际发送数据
                HAL_UART_Transmit_IT(data->handle, (uint8_t*)txBuffer, strlen(txBuffer));
            }
        }
    }
}
