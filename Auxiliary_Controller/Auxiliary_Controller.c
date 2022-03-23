// When including header files from the SDK, make sure you add the corresponding entry to the target_link_libraries list in CMakeLists.txt
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

bool relayState = true;
// OUT1 pin on the H-bridge connects to pin 4 on the relay
// OUT2 pin on the H-bridge connects to pin 3 on the relay
const uint turnRelayOnPin = 4; // Connects to IN1 pin on the H-bridge
const uint turnRelayOffPin = 5; // Connects to IN2 pin on the H-bridge

const uint testingSwitchPin = 26;

void Relay_Init();
void Relay_Control(bool openClose);
void Relay_Unit_Test();

// Code from Isean and Justin
const uint pwmPinInput = 27; //This pin is a PWM 2B type
// Try using pin 5 if 27 doesn't work
float measure_duty_cycle(uint gpio){
      // Only the PWM B pins can be used as inputs.
      assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);//skips over function if pin is not pwm b 
      uint slice_num = pwm_gpio_to_slice_num(gpio);// slices are used to measure frequency/duty cycles

      // Count once for every 100 cycles the PWM B input is high
      pwm_config cfg = pwm_get_default_config();//intitialization
      pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
      pwm_config_set_clkdiv(&cfg, 100);
      
      pwm_init(slice_num, &cfg, false);
      gpio_set_function(gpio, GPIO_FUNC_PWM);

      pwm_set_enabled(slice_num, true);
      sleep_ms(10);
      pwm_set_enabled(slice_num, false);
      float counting_rate = clock_get_hz(clk_sys) / 100;
      float max_possible_count = counting_rate * 0.01;
      return pwm_get_counter(slice_num) / max_possible_count;
}

// Alternate code based on 

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
    printf("Relay Control set true: %d", relayState);
}
