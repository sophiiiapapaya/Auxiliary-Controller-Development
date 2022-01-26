#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"


bool relayState = true;
uint turnRelayOnPin = 4;
uint turnRelayOffPin = 5;

//pages 115 & 118
//function switches on and off GPIO pin 4 & 5
//takes in a bool value to determine whether relay is open and closed to start
void relayControl(bool openClose) {

      //sets GPIO pins to initial position
      if(openClose)
      {
      gpio_put(turnRelayOnPin, openClose);
     //delay for 101 ms
      sleep_ms(101);
      //switches which pins are open and closed
      gpio_put(turnRelayOnPin, !openClose);
      }
      else
      {
      gpio_put(turnRelayOffPin, openClose);
      //delay for 101 ms
      sleep_ms(101);
      //switches which pins are open and closed
      gpio_put(turnRelayOffPin, !openClose);
      }
      //sets relay state to openClose
      relayState = openClose;
}


int main()
{
    stdio_init_all();
    gpio_init(turnRelayOnPin);
    gpio_init(turnRelayOffPin);
      
    gpio_set_dir(turnRelayOnPin, true);
    gpio_set_dir(turnRelayOffPin, true);
    
    stdio_usb_init();
    

    return 0;
}
