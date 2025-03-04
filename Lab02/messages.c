#include <stdio.h>
#include <string.h>
#include "messages.h"
//#include "cyBot_uart.h"
#include "uart.h"


void sendMessage(char message[50]) {

    int i = 0;

    while (1) {
        if (message[i] == '\0') {
            break;
        }
        uart_sendChar(message[i]);
        i++;
    }
    return;
}
