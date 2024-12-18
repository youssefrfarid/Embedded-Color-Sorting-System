#ifndef STEPPER_H
#define STEPPER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

// Define pins
#define STEP_PIN_1 3
#define STEP_PIN_2 4
#define STEP_PIN_3 5
#define STEP_PIN_4 6

// Function to initialize the stepper motor pins
void stepper_init(void);

// Function to step the stepper motor
void stepper_step(void);

#endif