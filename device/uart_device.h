#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H


#include "stdint.h"


typedef struct UART_Device
{
    char *name;
    int (*Init)(struct UART_Device *pDev);
    int (*Send)(struct UART_Device *pDev, uint8_t *datas, int len, int timeout_ms);
    int (*Recv)(struct UART_Device *pDev, uint8_t *data, int timeout_ms);
    void *priv_data;
}UART_DeviceType;

UART_DeviceType *Get_UART_Device(char *name);


#endif /* __UART_DEVICE_H */
