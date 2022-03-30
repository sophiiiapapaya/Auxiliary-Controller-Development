// Note that a uint8_t stores the equivalent of one byte of data
#include "stdint.h"

// Instead of a different struct for each message, the same struct can be reused but modified
    // to accomodate varying numbers of data bytes

typedef struct GCS_msg {
    uint8_t header;
    uint8_t num_bytes;
    uint8_t* bytes;
} GCS_msg;

// One of these is needed for each message type
    // header is the unique identifier for a message, where the MSB must be a one
        // we can assign header values in sequential order, since the value assigned to
        // a specific message does not matter as long as it is unique
    // num_bytes is simply the number of data bytes that the message uses
    // payload will be a pointer to an array of bytes which have been filled in with the required information
//Relay for VTOL Motors
#define RELAY_STATUS_MSG(payload) {.header = 0x80, \
                                   .num_bytes = 1, \
                                   .bytes = payload, }
//UGV Deployment
#define BAY_DOOR_STATUS_MSG(payload) {.header = 0x81, \
                                   .num_bytes = 2, \
                                   .bytes = payload, }
#define DROP_STATUS_MSG(payload) {.header = 0x82, \
                                   .num_bytes = 4, \
                                   .bytes = payload, }
//Diagnostic Messages
#define SERVO_VOLTAGE_STATUS_MSG(payload) {.header = 0x83, \
                                   .num_bytes = 1, \
                                   .bytes = payload, }
#define STEPPER_MOTOR_STATUS_MSG(payload) {.header = 0x84, \
                                   .num_bytes = 1, \
                                   .bytes = payload, }
#define RELAY_POWER_STATUS_MSG(payload) {.header = 0x85, \
                                   .num_bytes = 1, \
                                   .bytes = payload, }
#define CONTROLLER_STATUS_MSG(payload) {.header = 0x86, \
                                   .num_bytes = 2, \
                                   .bytes = payload, }
#define COMMDAN_RECEIVED_MSG(payload) {.header = 0x87, \
                                   .num_bytes = 1, \
                                   .bytes = payload, }