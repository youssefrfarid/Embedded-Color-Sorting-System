/**
 * @file color.c
 * @brief Implementation of TCS3200 color sensor interfacing with Raspberry Pi Pico W.
 *
 * This source file contains the definitions of functions declared in tcs3200.h.
 * It handles the configuration, frequency measurement, and color detection
 * using the TCS3200 color sensor.
 *
 * Usage:
 * 1. Initialize the sensor by calling `configure_sensor()`.
 * 2. Optionally perform calibration using `calibrate_sensor()`.
 * 3. In the main loop, call `color_detection()` to detect and print the color.
 *
 * Author: [Your Name]
 * Date: [Date]
 */

#include "color.h"
#include <stdio.h>

/* ================================ */
/*          Global Variables        */
/* ================================ */

volatile uint32_t pulse_count = 0; /**< Counts the number of pulses detected on OUT_PIN */

/* ================================ */
/*      Interrupt Service Routine   */
/* ================================ */

/**
 * @brief ISR for handling GPIO interrupts on OUT_PIN.
 *
 * Increments the pulse_count each time a falling edge is detected on OUT_PIN.
 *
 * @param gpio GPIO number that triggered the interrupt.
 * @param events Event flags indicating the type of interrupt.
 */
void gpio_callback(uint gpio, uint32_t events)
{
    if (gpio == OUT_PIN)
    {
        pulse_count++;
    }
}

/* ================================ */
/*          Function Definitions    */
/* ================================ */

void configure_sensor(void)
{
    // Initialize S0 and S1 for frequency scaling (20%)
    gpio_init(S0_PIN);
    gpio_set_dir(S0_PIN, GPIO_OUT);
    gpio_put(S0_PIN, false); // S0 = Low

    gpio_init(S1_PIN);
    gpio_set_dir(S1_PIN, GPIO_OUT);
    gpio_put(S1_PIN, true); // S1 = High

    // Initialize S2 and S3 as outputs with default low state
    gpio_init(S2_PIN);
    gpio_set_dir(S2_PIN, GPIO_OUT);
    gpio_put(S2_PIN, false); // Default state

    gpio_init(S3_PIN);
    gpio_set_dir(S3_PIN, GPIO_OUT);
    gpio_put(S3_PIN, false); // Default state

    // Initialize OUT_PIN as input with pull-up resistor
    gpio_init(OUT_PIN);
    gpio_set_dir(OUT_PIN, GPIO_IN);
    gpio_pull_up(OUT_PIN); // Enable pull-up resistor if needed

    // Set up interrupt on falling edge for OUT_PIN
    gpio_set_irq_enabled_with_callback(OUT_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

void select_color(bool s2_state, bool s3_state)
{
    gpio_put(S2_PIN, s2_state);
    gpio_put(S3_PIN, s3_state);
}

uint32_t measure_frequency(uint32_t measurement_time_ms)
{
    pulse_count = 0; // Reset pulse count

    // Allow some time for the sensor to stabilize after changing the color filter
    sleep_ms(50);

    // Wait for the measurement period
    sleep_ms(measurement_time_ms);

    // Calculate frequency (Hz)
    // frequency = pulse_count / (measurement_time_ms / 1000)
    // To avoid floating point, multiply first
    uint32_t frequency = (pulse_count * 1000) / measurement_time_ms;

    return frequency;
}

int color_detection(void)
{
    uint32_t red, green, blue;

    // Measure Red
    select_color(false, false);   // S2 = 0, S3 = 0
    red = measure_frequency(100); // Measure for 100ms

    // Measure Blue
    select_color(false, true); // S2 = 0, S3 = 1
    blue = measure_frequency(100);

    // Measure Green
    select_color(true, true); // S2 = 1, S3 = 1
    green = measure_frequency(100);

    // Print the RGB values
    printf("Red: %u Hz, Green: %u Hz, Blue: %u Hz\n", red, green, blue);

    // Normalize the values to mitigate lighting variations
    uint32_t max_val = red;
    if (green > max_val)
        max_val = green;
    if (blue > max_val)
        max_val = blue;

    if (max_val == 0)
    {
        printf("No color detected (all frequencies zero)\n");
        return 3;
    }

    float red_norm = (float)red / max_val;
    float green_norm = (float)green / max_val;
    float blue_norm = (float)blue / max_val;

    // Define threshold for color detection
    float threshold = 0.1; // Adjust based on calibration

    // Determine the color based on normalized values and thresholds
    if ((red_norm > green_norm + threshold) && (red_norm > blue_norm + threshold))
    {
        printf("Color detected: Red\n");
        return 0;
    }
    else if ((green_norm > red_norm + threshold) && (green_norm > blue_norm + threshold))
    {
        printf("Color detected: Green\n");
        return 1;
    }
    else if ((blue_norm > red_norm + threshold) && (blue_norm > green_norm + threshold))
    {
        printf("Color detected: Blue\n");
        return 2;
    }
    else
    {
        printf("Color detected: Unknown\n");
        return 3;
    }
}