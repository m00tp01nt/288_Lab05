/*
*
*   uart.c
*
*
*
*   @author
*   @date
*/

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart.h"

void uart_init(void){
	//TODO
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0b10;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0b10;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0b000010) == 0) {};     // page 406
  while ((SYSCTL_PRUART_R & 0b00000010) == 0) {};   // page 410

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0x3;                       // page 672

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x3;                         // page 683

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations    // page 1351, pb0
  GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the desired locations          // page 1351, pb1

//#error FINISH BAUD RATE CALCULATIONS
  // TODO Figure out baud rate stuff
  //calculate baud rate                             // page 896
  uint16_t iBRD = (int)(16000000 / (16 * 115200)); //use equations
  uint16_t fBRD = (((16000000 / (16 * 115200)) - iBRD) * 64 + 0.5); //use equations

  iBRD &= 0x0000FFFF;
  fBRD &= 0x0000FFFF;

  //turn off UART1 while setting it up
  UART1_CTL_R &= ~0b1;                 // page 918

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R |= iBRD;
#warning FIX FBRD CALCULATION
  //UART1_FBRD_R |= fBRD;
  UART1_FBRD_R |= 0b101100;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0b1100000;                        // page 916

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R &= ~0xF;                                 // page 939

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R |= 0x0301;                // page 918

}

void uart_sendChar(char data)
{
    // Wait to send
    while (UART1_FR_R & 0x20) {}

    // Send
    UART1_DR_R = data;
    //UART1_DR_R |= 0x00;
}

char uart_receive(void)
{
    char data = 0;

    while ((UART1_FR_R & UART_FR_RXFE)) {}

    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

void uart_sendStr(const char *data){
	//TODO for reference see lcd_puts from lcd.c file
}
