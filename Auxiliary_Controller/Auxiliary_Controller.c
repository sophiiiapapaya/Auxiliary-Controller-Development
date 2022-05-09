// When including header files from the SDK, make sure you add the corresponding entry to the target_link_libraries list in CMakeLists.txt
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

bool relayState = true;
// OUT1 pin on the H-bridge connects to pin 4 on the relay
// OUT2 pin on the H-bridge connects to pin 3 on the relay
const uint turnRelayOnPin = 4; // Connects to IN1 pin on the H-bridge
const uint turnRelayOffPin = 5; // Connects to IN2 pin on the H-bridge

const uint testingSwitchPin = 26;

void Relay_Init();
void Relay_Control(bool openClose);
void Relay_Unit_Test();

//// Put code for status LEDs here
const uint program_status_LED_pin = 6; // placeholder value
const uint relay_status_LED_pin = 7; // placeholder value
bool unresolved_error = false;

// switch on/off pin 6 & 7
void LED_stat(unresolved_error){
    // program_status_LED_pin ON by default

    // if an error is encoutered, unresolved_error == true
        if (!unresolved_error)
        { 
        // blink program status LED with period=1s 
        // involve setting up a timer with a separate callback fn
        gpio_put(program_status_LED_pin, );
        add_repeating_timer_ms (500, callback, NULL, &timer); 
        }
        else
        {
        // Timer stops toggling (cancel repeating timer fn) the pin once unresolved error == false
        bool cancelled = cancel_repeating_timer(&timer);
        }

}

void Led_Test() {
    while (gpio_get(testingSwitchPin));
    
    LED_stat(false); // !unresolved_error
    printf("LED status set true: %d", false);
    sleep_ms(5000);

    while (gpio_get(testingSwitchPin));

    LED_stat(true); // unresolved_error
    printf("LED status set false: %d", true);
}

// Function definitions here

////

int main() {
    stdio_init_all();
    stdio_usb_init();

    while(true) {
        sleep_ms(100);
    }

    return 0;
}

void Relay_Init() {
    gpio_init(turnRelayOnPin);
    gpio_init(turnRelayOffPin);
    gpio_set_dir(turnRelayOffPin, true);
    gpio_set_dir(testingSwitchPin, false);
}

//pages 115 & 118
//function switches on and off GPIO pin 4 & 5
//takes in a bool value to determine whether relay is open and closed to start
void Relay_Control(bool openClose) {
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

void Relay_Unit_Test() {
    while (gpio_get(testingSwitchPin));
    
    Relay_Control(true);
    printf("Relay Control set true: %d", relayState);
    sleep_ms(500);

    while (gpio_get(testingSwitchPin));

    Relay_Control(false);
    printf("Relay Control set false: %d", relayState);
}

