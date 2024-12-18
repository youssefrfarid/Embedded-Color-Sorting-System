#ifndef IR_H
#define IR_H

#include "pico/stdlib.h"

// Define the GPIO pin for the IR sensor output
#define IR_PIN 20

// Function to initialize the IR sensor
void ir_sensor_init(void);

// Function to check if an obstacle is detected
// Returns 0 if obstacle is detected, 1 otherwise
int ir_sensor_read(void);

#endif // IR_H