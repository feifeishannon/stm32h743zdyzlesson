#include "print_device.h"

FILE __stdout;

void _sys_exit(int x){
    x = x;
}

int fputc(int ch, FILE *f){
    HAL_UART_Transmit_IT(&huart1,(uint8_t *)&ch,1);
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}
    return ch;

}