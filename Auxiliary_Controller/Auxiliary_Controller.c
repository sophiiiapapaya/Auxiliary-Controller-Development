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
