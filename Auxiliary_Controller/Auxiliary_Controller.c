#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

uint relayState = relayState;

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

void gpioControl(int gpioNum, bool offOn) {
  if(gpioNum > 0 && gpioNum < 35 && gpioNum != 3 && gpioNum != 8 && gpioNum != 13 && gpioNum != 18 && gpioNum != 23 && gpioNum != 28 && gpioNum != 33){
      gpio_set_dir(gpioNumber, offOn);
      sleep_ms(101);
      relayState = gpio_get_dir(gpioNumber);
  }
  else{
    relayState = -1;
  }
}


//line 16: user inputs GPIO pin, int, and wheter they want to open channel, bool.
//line 17: checks if GPIO pin is a valid pin
//line 18: turns on/off the gpio pin depending on what the user wants, through bool parameter.
//line 19: wait 101 ms for physical delay
//line 20: updates global variable with dir direction, 1 for out, 0 for in.
//line 23: if an invalid pin is inputed, the global variable is set to -1 to indicate an error.

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
