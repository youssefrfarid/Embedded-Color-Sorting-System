#ifndef COLOR_H
#define COLOR_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

/**
 * @file color.h
 * @brief Header file for interfacing the TCS3200 color sensor with Raspberry Pi Pico W.
 *
 * This header file contains definitions, function prototypes, and configurations
 * necessary for operating the TCS3200 color sensor.
 */

/* ================================ */
/*           Pin Definitions        */
/* ================================ */

// Define the GPIO pins connected to the TCS3200 sensor
#define S0_PIN 15   /**< GPIO pin connected to S0 */
#define S1_PIN 14   /**< GPIO pin connected to S1 */
#define S2_PIN 16  /**< GPIO pin connected to S2 */
#define S3_PIN 17  /**< GPIO pin connected to S3 */
#define OUT_PIN 18 /**< GPIO pin connected to OUT */

/* ================================ */
/*          Function Prototypes      */
/* ================================ */

/**
 * @brief Configures the GPIO pins and initializes the TCS3200 sensor.
 *
 * Sets up the frequency scaling, initializes control pins, and configures
 * the output pin with interrupts.
 */
void configure_sensor(void);

/**
 * @brief Selects the color filter by setting S2 and S3 pins.
 *
 * @param s2_state Boolean value to set S2 pin (true for high, false for low).
 * @param s3_state Boolean value to set S3 pin (true for high, false for low).
 */
void select_color(bool s2_state, bool s3_state);

/**
 * @brief Measures the frequency output from the TCS3200 sensor.
 *
 * Utilizes interrupts to count the number of pulses within a specified time frame.
 *
 * @param measurement_time_ms Duration for frequency measurement in milliseconds.
 * @return uint32_t Frequency in Hertz (Hz).
 */
uint32_t measure_frequency(uint32_t measurement_time_ms);

/**
 * @brief Detects the color by measuring frequencies for red, green, and blue.
 *
 * Measures the frequency corresponding to each color filter and determines
 * the dominant color based on normalized values and defined thresholds.
 *
 * @return int Detected color code:
 *             0 - Red,
 *             1 - Green,
 *             2 - Blue,
 *             3 - Unknown
 */
int color_detection(void);

/**
 * @brief Calibrates the TCS3200 sensor.
 *
 * Placeholder for calibration routines to establish baseline frequency values
 * for accurate color detection. Implementation depends on specific setup and requirements.
 */

#endif // TCS3200_H
