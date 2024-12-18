#include "stepper.h"

void stepper_init()
{
    gpio_init(STEP_PIN_1);
    gpio_init(STEP_PIN_2);
    gpio_init(STEP_PIN_3);
    gpio_init(STEP_PIN_4);

    gpio_set_dir(STEP_PIN_1, GPIO_OUT);
    gpio_set_dir(STEP_PIN_2, GPIO_OUT);
    gpio_set_dir(STEP_PIN_3, GPIO_OUT);
    gpio_set_dir(STEP_PIN_4, GPIO_OUT);

    printf("Stepper motor initialized\n");
}

void stepper_step()
{
    gpio_put(STEP_PIN_4, 0);
    gpio_put(STEP_PIN_1, 1);
    sleep_ms(2);
    gpio_put(STEP_PIN_1, 0);
    gpio_put(STEP_PIN_2, 1);
    sleep_ms(2);
    gpio_put(STEP_PIN_2, 0);
    gpio_put(STEP_PIN_3, 1);
    sleep_ms(2);
    gpio_put(STEP_PIN_3, 0);
    gpio_put(STEP_PIN_4, 1);
    sleep_ms(2);
}