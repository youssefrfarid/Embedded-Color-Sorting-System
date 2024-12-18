#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 28     // GPIO pin connected to the servo signal
#define PWM_FREQUENCY 50 // 50 Hz PWM frequency (20 ms period)
#define MIN_DUTY 1000    // 1 ms pulse for 0 degrees
#define MAX_DUTY 2000    // 2 ms pulse for 180 degrees
uint slice_num;
// Function to initialize the servo
void servo_init(void);

// Function to calculate duty cycle for a given angle
uint16_t calculate_duty_cycle(float angle);

// Function to set the servo angle
void set_servo_angle(uint slice_num, float angle);

#endif