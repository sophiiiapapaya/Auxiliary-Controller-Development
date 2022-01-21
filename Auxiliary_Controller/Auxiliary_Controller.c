#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"


bool relayState = false;
uint GPIO4 = 4;
uint GPIO5 = 5;

//pages 115 & 118
//function switches on and off GPIO pin 4 & 5
//takes in a bool value to determine which pin is open and closed to start
void relayControl(bool openClose) {

      //sets GPIO pins to initial position
      gpio_put(GPIO4, openClose);
      gpio_put(GPIO5, !openClose);
     //delay for 101 ms
      sleep_ms(101);
      //switches which pins are open and closed
      gpio_put(GPIO4, !openClose);
      gpio_put(GPIO5, openClose);
      //sets relay state to openClose
      relayState = openClose;
}


int main()
{
    stdio_init_all();
    gpio_init(GPIO4);
    gpio_init(GPIO5);
      
    gpio_set_dir(GPIO4, true);
    gpio_set_dir(GPIO5, false);

    return 0;
}
