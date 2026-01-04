#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_sleep.h"
#include "esp_system.h"

#define LED_PIN GPIO_NUM_2

#define ACTIVE_TIME_MS        4000
#define LIGHT_SLEEP_SEC       3
#define DEEP_SLEEP_SEC        6

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    /* ============== DETECCIÓN DE CAUSA DE DESPERTAR ============== */
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
        printf("[WAKEUP] Activado por temporizador\n");
    } else {
        printf("[WAKEUP] Encendido inicial del sistema\n");
    }

    printf("[ESTADO] Sistema en modo activo\n");
    gpio_set_level(LED_PIN, 1);

    printf("[TAREA] Ejecutando proceso principal...\n");
    vTaskDelay(pdMS_TO_TICKS(ACTIVE_TIME_MS));

    // LIGHT SLEEP 
    printf("[ENERGÍA] Entrando en Light Sleep (%d s)\n", LIGHT_SLEEP_SEC);
    gpio_set_level(LED_PIN, 0);   // LED apagado = bajo consumo

    esp_sleep_enable_timer_wakeup(LIGHT_SLEEP_SEC * 1000000ULL);
    esp_light_sleep_start();

    printf("[ENERGÍA] Salida de Light Sleep\n");
    gpio_set_level(LED_PIN, 1);

    vTaskDelay(pdMS_TO_TICKS(2000));  // breve actividad posterior

    // DEEP SLEEP
    printf("[ENERGÍA] Preparando Deep Sleep (%d s)\n", DEEP_SLEEP_SEC);
    gpio_set_level(LED_PIN, 0);

    esp_sleep_enable_timer_wakeup(DEEP_SLEEP_SEC * 1000000ULL);
    esp_deep_sleep_start();
}
