#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H

#include "stm32h7xx_hal.h"
#include "stdint.h"


struct UART_Device
{
    char *name;
    int (*Init)(int which, int baud, int datas, char parity, int stop);
    int (*Send)(uint8_t *datas, int len, int timeout_ms);
    int (*Init)(uint8_t *data, int timeout_ms);
    UART_HandleTypeDef *handle;
};



#endif /* __UART_DEVICE_H */
