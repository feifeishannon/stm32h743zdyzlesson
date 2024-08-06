#ifndef __PRINT_DEVICE_H
#define __PRINT_DEVICE_H

#include "main.h"
#include "uart_device.h"

typedef struct __FILE {
    int handle;
} FILE;

extern FILE __stdout;

void _sys_exit(int x);
int fputc(int ch, FILE *f);

/**
 * @brief  转为标准行发送接口
 * @note   依赖于实例化的UARTdevice
 * @retval 
 */
#define println(...)    UARTdevice->Sendln(__VA_ARGS__)

// #define println(...)                        \
//     do {                                    \
//         UARTdevice->Sendln(__VA_ARGS__);    \
//     } while (0)

#endif /* __PRINT_DEVICE_H */
