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

// The default pwm frequency for an AUX channel on the Pixhawk is 50 Hz
// Pulse width can be between 1000 us and 2000 us; a tolerance of 15 
    // microseconds is included
#define MIN_PULSE_WIDTH_US (1000 - 15)
#define MAX_PULSE_WIDTH_US (2000 + 15)

// Needs to be declared globally to be shared by multiple functions
const uint pwmPinInput = 27; //This pin is a PWM 2B type
// last_rising_time_us tracks the time in microseconds when the last rising edge was detected
    // on pin 27
volatile uint64_t last_rising_time_us;
// last_pulse_width_us tracks the duration in microseconds of the last complete pulse received
    // on pin 27
volatile uint64_t last_pulse_width_us;
// pwm_active is true while a PWM signal is being applied to pin 27, and switches to false
    // within 21 milliseconds of the signal stopping 
volatile bool pwm_active;
// Serves as a catch-all for errors encountered in the interrupt service routine functions
volatile bool pwm_error_flag;

// The alarm callback function which sets the value of pwm_active
int64_t pwm_active_check(alarm_id_t id, void *user_data) {
    // If too much time has passed since the last rising edge and the pin is low, 
        // then the signal must have stopped
    if ((time_us_64() - last_rising_time_us > 20000) && !gpio_get(pwmPinInput)) {
        pwm_active = false;
    }
    return 0;
}
// The interrupt service routine which sets the value of last_pulse_width_us
    // Note that gpio stores the pin number that the interrupt occured on and
    // events stores what kind of change took place on the pin.
    // If there is ever a need to set an interrupt on another pin, this function
    // will need to be modified so that the code only runs if the interrupt
    // occured on pin 27
void pwm_recv_callback(uint gpio, uint32_t events) {
    // Disable interrupts on this pin temporarily
    gpio_set_irq_enabled(pwmPinInput, 0xFFFFFFFF, false);
    switch(events) {
        case GPIO_IRQ_EDGE_RISE:
            // Set flag to indicate a pwm signal is being received (only if this is
                // the first pulse of the signal)
            if (!pwm_active) {
                pwm_active = true;
            }
            // Save the current time to measure the pulse width at the end of the pulse
            last_rising_time_us = time_us_64();
            // Also start a timer that is greater than the period of the PWM signal to
                // detect if the signal has stopped; this will check if the next expected
                // pulse is received
            add_alarm_in_us(21000, pwm_active_check, NULL, false);
            // Set next interrupt to trigger on the falling edge of the pulse
            gpio_set_irq_enabled_with_callback(pwmPinInput, GPIO_IRQ_EDGE_FALL, true, pwm_recv_callback);
            break;
        case GPIO_IRQ_EDGE_FALL:
            // Get the time elapsed since the pulse began
            last_pulse_width_us = time_us_64() - last_rising_time_us;
            // If the measured pulse width does not fall within the expected range of values
                // that the Pixhawk can output, then there is an error. Realistically, there
                // needs to be a few microseconds of tolerance in the expected range of values
                // due to the limited accuracy of the Pixhawk's PWM generation. Right now, the
                // tolerance is 15 microseconds
            if (last_pulse_width_us < MIN_PULSE_WIDTH_US || last_pulse_width_us > MAX_PULSE_WIDTH_US) {
                pwm_error_flag = true;
            }
            // Set interrupt to trigger on the rising edge of the next pulse
            gpio_set_irq_enabled_with_callback(pwmPinInput, GPIO_IRQ_EDGE_RISE, true, pwm_recv_callback);
            break;
        default:
            // ERROR
            pwm_error_flag = true;
            gpio_set_irq_enabled_with_callback(pwmPinInput, GPIO_IRQ_EDGE_RISE, true, pwm_recv_callback);
            break;
    }
}

int main() {
    stdio_init_all();
    stdio_usb_init();

    // Initialization for receiving PWM signal
    gpio_init(pwmPinInput);
    gpio_set_dir(pwmPinInput, false); // Input
    last_rising_time_us = 0;
    last_pulse_width_us = 0;
    pwm_active = false;
    pwm_error_flag = false;
    // BEgin waiting for a PWM signal by setting an interrupt on rising edges;
        // have it call the function pwm_recv_callback
    gpio_set_irq_enabled_with_callback(pwmPinInput, GPIO_IRQ_EDGE_RISE, true, pwm_recv_callback);

    while(true) {
        printf("Last pulse width: %lld\n", last_pulse_width_us);
        //printf("Last rising time: %lld\n", last_rising_time_us);
        printf(pwm_active ? "Active" : "Not active");
        printf("\n");
        if (pwm_error_flag) {
            printf("Error in pwm_recv_callback");
            pwm_error_flag = false;
        }
        // Do not print messages too frequently
        busy_wait_us(100000);
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
