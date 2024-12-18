#include "servo.h"

void servo_init(void)
{
    // Configure GPIO pin for PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    // Get PWM slice associated with the GPIO pin
    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Configure PWM with a frequency of 50 Hz (20 ms period)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // Clock divider for 50 Hz

    // Set PWM top to achieve a 20 ms period (1 µs resolution)
    pwm_set_wrap(slice_num, 20000);
    pwm_init(slice_num, &config, true);
    printf("Servo initialized\n");
}

// Function to calculate duty cycle for a given angle
uint16_t calculate_duty_cycle(float angle)
{
    // Map the angle (0°-180°) to the duty cycle range (1350-8200)
    return MIN_DUTY + (uint16_t)((angle / 180.0f) * (MAX_DUTY - MIN_DUTY));
}

// Function to set the servo angle by changing PWM duty cycle
void set_servo_angle(uint slice_num, float angle)
{
    uint16_t duty = calculate_duty_cycle(angle);
    pwm_set_gpio_level(SERVO_PIN, duty); // Set PWM duty cycle
    printf("Moving to angle: %.1f degrees\n", angle);
}