#include "ir.h"

// Function to initialize the IR sensor
void ir_sensor_init(void)
{
    gpio_init(IR_PIN);
    gpio_set_dir(IR_PIN, GPIO_IN);
    printf("IR sensor initialized\n");
}

// Function to check if an obstacle is detected
// Returns 0 if obstacle is detected, 1 otherwise
int ir_sensor_read(void)
{
    int res = gpio_get(IR_PIN);
    if (res == 0)
    {
        printf("Object Detected by IR!!\n");
    }
    return res;
}
