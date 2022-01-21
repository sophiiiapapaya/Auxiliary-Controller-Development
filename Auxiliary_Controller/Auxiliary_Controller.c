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

void relayControl(bool openClose) {
      int GPIO4 = 4;
      int GPIO5 = 5;
      gpio_put(GPIO4, openClose);
      gpio_put(GPIO5, !openClose);
      sleep_ms(101);
      gpio_put(GPIO4, !openClose);
      gpio_put(GPIO5, openClose);
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
