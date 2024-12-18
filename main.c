#include <stdio.h>
#include "pico/stdlib.h"
#include "ir.h"
#include "ir.c"
#include "color.h"
#include "color.c"
#include "ultrasonic.h"
#include "ultrasonic.c"
#include "stepper.h"
#include "stepper.c"
#include "FreeRTOS.h"
#include "servo.h"
#include "servo.c"
#include "dc.h"
#include "dc.c"
#include "task.h"
#include "event_groups.h"

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS(); // Disable interrupts to prevent further memory allocation attempts
    for (;;)
    {
    } // Enter an infinite loop to halt the system
}

//  function to handle stack overflows
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // You can print an error message or handle the stack overflow in other ways
    printf("Stack overflow in task: %s\n", pcTaskName);
    // Optionally, halt the system or take corrective action
    taskDISABLE_INTERRUPTS(); // Disable interrupts to stop any further execution
    for (;;)
    {
        // Infinite loop to halt the system
    }
}

EventGroupHandle_t xEventGroup; // Event group for synchronization
#define EVENT_CONVEYOR_COMPLETE (1 << 0)

void vSensorTask(void *pvParameters)
{
    // Initialize sensors and actuators
    configure_sensor();
    ultrasonic_init();
    servo_init();
    stepper_init();

    while (1)
    {
        // Wait for conveyor task to signal completion
        xEventGroupWaitBits(
            xEventGroup,
            EVENT_CONVEYOR_COMPLETE,
            pdTRUE,       // Clear the bits before returning
            pdTRUE,       // Wait for all bits
            portMAX_DELAY // Wait indefinitely
        );
        printf("Reading Sensors!\n");
        // Read sensors
        float distance = ultrasonic_get_distance_cm();
        int color = color_detection();

        // Move servo based on color
        if (color == 0)
        {
            if (distance >= 4)
            {
                set_servo_angle(slice_num, 5);
            }
            else
            {
                set_servo_angle(slice_num, 60);
            }
        }
        else if (color == 2)
        {
            if (distance >= 4)
            {
                set_servo_angle(slice_num, 115);
            }
            else
            {
                set_servo_angle(slice_num, 175);
            }
        }

        // Wait for 2 seconds
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Run stepper motor for 2 seconds
        TickType_t startTime = xTaskGetTickCount();
        TickType_t runTime = pdMS_TO_TICKS(5000); // 2 seconds in ticks
        printf("Running Stepper motor\n");
        while ((xTaskGetTickCount() - startTime) < runTime)
        {
            stepper_step();
        }
        printf("Stepper motor stopped\n");
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

// Conveyor Task
void vConveyorTask(void *pvParameters)
{
    ir_sensor_init();
    dc_init();

    while (1)
    {
        // Wait for IR detection
        if (ir_sensor_read() == 0)
        {
            // Wait for 2 seconds
            vTaskDelay(pdMS_TO_TICKS(2000));

            // Run stepper motor for 2 seconds
            TickType_t startTime = xTaskGetTickCount();
            TickType_t runTime = pdMS_TO_TICKS(1000); // 2 seconds in ticks
            printf("Running DC motor\n");
            while ((xTaskGetTickCount() - startTime) < runTime)
            {
                dc_run();
            }
            dc_stop();
            printf("DC motor stopped\n");

            vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
            // Signal that the conveyor task is complete
            xEventGroupSetBits(xEventGroup, EVENT_CONVEYOR_COMPLETE);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Small delay to prevent task hogging
    }
}

int main(void)
{
    stdio_init_all();

    // Create the event group
    xEventGroup = xEventGroupCreate();
    if (xEventGroup == NULL)
    {
        printf("Failed to create event group.\n");
        while (1)
        {
        }
    }

    // Create tasks
    BaseType_t result;
    result = xTaskCreate(vConveyorTask, "ConveyorTask", 512, NULL, 1, NULL);
    if (result != pdPASS)
    {
        printf("Failed to create ConveyorTask.\n");
        while (1)
        {
        }
    }

    result = xTaskCreate(vSensorTask, "SensorTask", 512, NULL, 1, NULL);
    if (result != pdPASS)
    {
        printf("Failed to create SensorTask.\n");
        while (1)
        {
        }
    }

    printf("Starting scheduler\n");
    vTaskStartScheduler();

    // Should never reach here
    for (;;)
        ;
}