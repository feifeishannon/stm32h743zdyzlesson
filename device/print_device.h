#ifndef __PRINT_DEVICE_H
#define __PRINT_DEVICE_H

#include "main.h"

typedef struct __FILE {
    int handle;
} FILE;

extern FILE __stdout;

void _sys_exit(int x);
int fputc(int ch, FILE *f);


#endif /* __PRINT_DEVICE_H */
