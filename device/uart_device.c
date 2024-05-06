#include "uart_device.h"
#include "FreeRTOS.h"

extern UART_HandleTypeDef huart1;


struct UART_Data 
{
    UART_HandleTypeDef *handle;
    SemaphoreHandle_t xTxSem;
};

struct UART_Device g_uart1 = {
    "stm32_uart1",
    stm32_uart_init,
    stm32_uart_send,
    stm32_uart_recv,
    &g_stm32_uart1_data,
};

static struct UART_Data g_stm32_uart1_data = {
    &huart1,
};

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart){
    if(huart == &huart1) {

    }
}

static int stm32_uart_init(struct UART_Device *pDev, uint8_t *datas, int len, int timeout_ms){

}

static int stm32_uart_send(struct UART_Device *pDev, uint8_t *datas, int len, int timeout_ms){
    HAL_UART_Transmit_IT(pDev->handle, datas, len, timeout_ms);
}

static int stm32_uart_recv(struct UART_Device *pDev, uint8_t *data, int timeout_ms){

}

main(){
    struct UART_Device *p = &g_uart1;
    
    p->Init();
    p->Send(p, "hello world\r\n", 15, 100);
}