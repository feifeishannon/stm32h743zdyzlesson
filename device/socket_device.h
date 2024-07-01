#ifndef __SOCKET_DEVICE_H
#define __SOCKET_DEVICE_H


#include "stdint.h"


typedef struct Socekt_Device
{
    char *name;
    int (*Init)(struct Socekt_Device *pDev);
    int (*Send)(struct Socekt_Device *pDev, uint8_t *datas, int timeout_ms);
    int (*Recv)(struct Socekt_Device *pDev, uint8_t *data, int timeout_ms);
    void *priv_data;
}Socekt_DeviceType;

extern const u8_t lwiperf_txbuf_const[1600];


Socekt_DeviceType *Get_Socekt_Device(char *name);


#endif /* __SOCKET_DEVICE_H */
