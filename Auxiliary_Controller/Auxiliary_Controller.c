#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

uint relayState = relayState;

//pages 115 & 118
//function switches on and off GPIO pin 4 & 5
//takes in a bool value to determine which pin is open and closed to start
void relayControl(bool openClose) {
      int GPIO4 = 4;
      int GPIO5 = 5;
      //sets GPIO pins to initial position
      gpio_put(GPIO4, openClose);
      gpio_put(GPIO5, !openClose);
     //delay for 101 ms
      sleep_ms(101);
      //switches which pins are open and closed
      gpio_put(GPIO4, !openClose);
      gpio_put(GPIO5, openClose);
      //sets relay state to the state of GPIO pin 4
      relayState = gpio_get(GPIO4);
}


int main()
{
    stdio_init_all();


    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);

    while (1) {
        puts("Hello World\n");
        sleep_ms(1000);
    }

    return 0;
}
