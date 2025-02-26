/**
 * @author
 * @date 1/28/2025
 *
 * Description: Includes timer.h, lcd.h
 */

#include <stdio.h>
#include <string.h>

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"

#include "messages.h"
#include "servo.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"

#define LATERAL_SPEED 250


int main (void) {

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                  // and enables time functions (e.g. timer_waitMillis)
    lcd_init();   // Initialize the LCD screen.  This also clears the screen.

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    cyBot_uart_init(); // Initializes the UART stuff





    oi_free(sensor_data);
}
