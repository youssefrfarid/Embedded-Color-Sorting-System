#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Define the GPIO pins for the ultrasonic sensor
#define TRIGGER_PIN 7
#define ECHO_PIN 8

// Function to initialize the ultrasonic sensor pins
void ultrasonic_init(void);

// Function to measure the distance
// Returns the distance in centimeters
float ultrasonic_get_distance_cm(void);

#endif // ULTRASONIC_H