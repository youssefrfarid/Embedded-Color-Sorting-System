#ifndef DC_H
#define DC_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Define pins
#define POTENTIOMETER_PIN 26 // ADC0
#define IN1_PIN 1
#define IN2_PIN 0 // hamra
#define EN_PIN 2  // PWM output

uint pwm_slice_num;

void dc_init(void);

void dc_run(void);

void dc_stop(void);

#endif