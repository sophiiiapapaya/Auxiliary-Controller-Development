// When including header files from the SDK, make sure you add the corresponding entry to the target_link_libraries list in CMakeLists.txt
#include <stdio.h>
#include <time.h>
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
int led_val = 0; 

// Global var || define in main fn and pass a pointer to the add_repeating_timer_ms fn
// defining before calling add_repeating_timer_ms would remove it from the memory
struct repeating_timer timer; 

// switch on/off pin 6 & 7
void LED_stat(){
    gpio_init(program_status_LED_pin); 
    gpio_init(relay_status_LED_pin);
    gpio_set_dir(program_status_LED_pin, true);  // true for out, false for in
    gpio_set_dir(relay_status_LED_pin, true);
    // program_status_LED_pin ON by default
    gpio_put(program_status_LED_pin, 1);  

    /*
    <Jeremy> check both whether the timer is already active
    and whether it needs to be active (based on the variable unresolved_error).
    If it needs to be active and it already is, then nothing needs to be done.
    If it needs to be active and it is not, then call add_repeating_timer_ms.
    If it needs to not be active and it is active, then call cancel_repeating_timer.
    If it needs to not be active and it is not active, then nothing needs to be done.
    */

    /*
    <Jeremy> The function add_repeating_timer_ms returns true or false depending on
    whether there were enough hardware resources available to create the timer;
    it cannot determine whether the timer has already been initialized. In order
    to check whether the timer is active you need to look at the timer object
    itself (in this case, the object named timer). I think that the alarm ID can
    be used to determine this but you will need to confirm by checking if the 
    value gets reset when the repeating timer is cancelled. (see here: https://raspberrypi.github.io/pico-sdk-doxygen/structrepeating__timer.html)
    */

   //see p206, lines 25-28
   // p251 4.2 cancel_alarm
    
    // is an alarm and a timer referring to the same thing

    // check if var timer is active
    // if a timer exist (how?), get the value of the timer to check if it is active
    if (timer != NULL) // check if timer exists 
    {
        // bool checktimer >> timer active/inactive

    }

    // inverting the value is not needed
    if (unresolved_error == true) 
    { 
        // blink program status LED with period=1s until error resolved
        // involve setting up a timer with a separate callback fn

        // if there's no active timer, add a timer
        if (checktimer != true)
        {
            add_repeating_timer_ms (500, repeating_timer_callback, NULL, &timer); 
            // do I need to check what value it returns?
        }

        // Implement error handling
    }
    else
    {
        if (checktimer == true)
        {
            // Timer stops toggling (cancel repeating timer fn) the pin once unresolved error == false
            bool cancelled = cancel_repeating_timer(&timer);
        }
        

        // Program status LED ON once it stops toggling
        gpio_put(program_status_LED_pin, 1); 
    }
        

    gpio_put(relay_status_LED_pin, !relayState); 

}

bool repeating_timer_callback(struct repeating_timer *t){
    led_val = 1 - led_val; 
    gpio_put(program_status_LED_pin, led_val);
    printf("LED toggled.\n"); // for testing purpose only
    // printing lines slows down the code
    return true; 
}

void Led_Test() {
    // while (gpio_get(program_status_LED_pin));
    // LED pins are output only, unlike relay control
    
    unresolved_error = true; 
    LED_stat(); 
    printf("Program staus LED toggles");
    // set a longer delay for easier testing
    sleep_ms(1000);

    unresolved_error = false; 
    LED_stat(); 
    printf("Program status LED is ON");
    
    // -----------------------
    // Relay_Control(true);
    // modify relayState directly when testing
    relayState = true; 
    printf("Relay staus LED set OFF: %d", !relayState);
    sleep_ms(1000);

    relayState = false; 
    printf("Relay status LED set ON: %d", !relayState);
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

