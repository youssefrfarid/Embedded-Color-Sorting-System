#include "dc.h"

void dc_init(void)
{
    // Initialize ADC
    adc_init();
    adc_gpio_init(POTENTIOMETER_PIN);
    adc_select_input(0); // ADC0 (GPIO 26)

    // Initialize GPIO pins for motor control
    gpio_init(IN1_PIN);
    gpio_set_dir(IN1_PIN, GPIO_OUT);
    gpio_init(IN2_PIN);
    gpio_set_dir(IN2_PIN, GPIO_OUT);

    // Set motor direction (e.g., forward)
    gpio_put(IN1_PIN, true);
    gpio_put(IN2_PIN, false);

    // Set the PWM output pin
    gpio_set_function(EN_PIN, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to EN_PIN
    pwm_slice_num = pwm_gpio_to_slice_num(EN_PIN);

    // Set PWM frequency (e.g., 1 kHz)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // 125 MHz / 125 = 1 MHz base frequency
    pwm_init(pwm_slice_num, &config, true);

    // Enable PWM
    pwm_set_enabled(pwm_slice_num, true);

    printf("DC Motor Initialized\n");
}

void dc_run(void)
{
    // Read the potentiometer value (12-bit resolution: 0 - 4095)
    uint16_t pot_value = adc_read();

    // Map the potentiometer value to PWM duty cycle (0 - 65535)
    uint16_t pwm_value = pot_value * 65535 / 4095;

    // Set the PWM duty cycle
    pwm_set_gpio_level(EN_PIN, pwm_value);
}

void dc_stop(void)
{
    // Set the PWM duty cycle to 0 to stop driving the motor
    pwm_set_gpio_level(EN_PIN, 0);
}
