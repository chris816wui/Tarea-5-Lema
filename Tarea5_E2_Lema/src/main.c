#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2


static int sensor_value = 0;

//   TAREA 1: Sensor virtual

void task_sensor(void *pvParameters)
{
    while (1) {
        sensor_value += 5;  // Lectura incremental
        printf("[SENSOR] Valor actualizado: %d\n", sensor_value);

        /* Temporización periódica */
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 segundo
    }
}

//   TAREA 2: Control de LED

void task_led(void *pvParameters)
{
    int led_state = 0;

    while (1) {
        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);

        printf("[LED] Estado cambiado a: %s\n",
               led_state ? "ENCENDIDO" : "APAGADO");

        /* Frecuencia diferente al sensor */
        vTaskDelay(pdMS_TO_TICKS(700)); // 700 ms
    }
}

//   TAREA 3: Monitor del sistema

void task_monitor(void *pvParameters)
{
    while (1) {
        printf("[MONITOR] Sistema operativo | Sensor=%d\n", sensor_value);

        /* Envío periódico de información */
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 segundos
    }
}


void app_main(void)
{
    /* Configuración del GPIO del LED */
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    printf("=== Sistema multitarea iniciado ===\n");

    /* Creación de tareas FreeRTOS */

    xTaskCreate(
        task_sensor,
        "Task_Sensor",
        2048,
        NULL,
        2,      // Prioridad media
        NULL
    );

    xTaskCreate(
        task_led,
        "Task_LED",
        2048,
        NULL,
        1,      // Prioridad baja
        NULL
    );

    xTaskCreate(
        task_monitor,
        "Task_Monitor",
        2048,
        NULL,
        3,      // Prioridad alta
        NULL
    );
}
