#include "ultrasonic.h"

void ultrasonic_init()
{
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);
    gpio_put(TRIGGER_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    printf("Ultrasonic sensor initialized\n");
}

float ultrasonic_get_distance_cm()
{
    // Send a 10us pulse to trigger the sensor
    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);

    // Wait for the echo to go high
    while (gpio_get(ECHO_PIN) == 0)
    {
        tight_loop_contents();
    }
    absolute_time_t start_time = get_absolute_time();

    // Wait for the echo to go low
    while (gpio_get(ECHO_PIN) == 1)
    {
        tight_loop_contents();
    }
    absolute_time_t end_time = get_absolute_time();

    // Calculate the pulse duration in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Speed of sound is approximately 34300 cm/s
    // Distance = (pulse_duration in seconds) * (speed of sound) / 2
    float distance_cm = (pulse_duration * 0.0343f) / 2.0f;
    printf("Distance: %.2f cm\n", distance_cm);
    return distance_cm;
}