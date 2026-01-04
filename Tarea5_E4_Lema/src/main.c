#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT        UART_NUM_0
#define UART_TX_PIN      GPIO_NUM_17
#define UART_RX_PIN      GPIO_NUM_16
#define UART_BAUD_RATE   115200
#define UART_BUF_SIZE    256

#define LED_PIN          GPIO_NUM_2

static int command_counter = 0;
static int led_state = 0;
static int sensor_value = 0;

typedef enum {
    CMD_STATUS,
    CMD_LED_ON,
    CMD_LED_OFF,
    CMD_INFO,
    CMD_RESET,
    CMD_INVALID
} command_t;

static QueueHandle_t command_queue;

void trim_newline(char *str)
{
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

void uart2_init(uint32_t baudrate)
{
    uart_config_t uart_cfg = {
        .baud_rate  = baudrate,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_PORT, &uart_cfg);
    uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_PORT,
                        UART_BUF_SIZE,
                        UART_BUF_SIZE,
                        0,
                        NULL,
                        0);
}

void send_response(const char *msg)
{
    uart_write_bytes(UART_PORT, msg, strlen(msg));
}

//tarea UART
void task_uart_rx(void *pvParameters)
{
    uint8_t rx_buffer[UART_BUF_SIZE];
    command_t cmd;

    send_response(">> UART listo. Ingrese comandos\r\n");

    while (1) {
        int len = uart_read_bytes(
            UART_PORT,
            rx_buffer,
            UART_BUF_SIZE - 1,
            50 / portTICK_PERIOD_MS
        );

        if (len > 0) {
            rx_buffer[len] = '\0';
            trim_newline((char *)rx_buffer);
            command_counter++;

            if (strcmp((char *)rx_buffer, "status") == 0)
                cmd = CMD_STATUS;
            else if (strcmp((char *)rx_buffer, "led on") == 0)
                cmd = CMD_LED_ON;
            else if (strcmp((char *)rx_buffer, "led off") == 0)
                cmd = CMD_LED_OFF;
            else if (strcmp((char *)rx_buffer, "info") == 0)
                cmd = CMD_INFO;
            else if (strcmp((char *)rx_buffer, "reset") == 0)
                cmd = CMD_RESET;
            else
                cmd = CMD_INVALID;

            xQueueSend(command_queue, &cmd, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

//tarea control
void task_control(void *pvParameters)
{
    command_t rx_cmd;
    char msg[128];

    while (1) {
        if (xQueueReceive(command_queue, &rx_cmd, portMAX_DELAY)) {

            switch (rx_cmd) {

                case CMD_STATUS:
                    send_response("[OK] Sistema operativo\r\n");
                    break;

                case CMD_LED_ON:
                    led_state = 1;
                    gpio_set_level(LED_PIN, 1);
                    send_response("[OK] LED encendido\r\n");
                    break;

                case CMD_LED_OFF:
                    led_state = 0;
                    gpio_set_level(LED_PIN, 0);
                    send_response("[OK] LED apagado\r\n");
                    break;

                case CMD_INFO:
                    snprintf(msg, sizeof(msg),
                             "[INFO] Cmd=%d | LED=%s | Sensor=%d\r\n",
                             command_counter,
                             led_state ? "ON" : "OFF",
                             sensor_value);
                    send_response(msg);
                    break;

                case CMD_RESET:
                    command_counter = 0;
                    sensor_value = 0;
                    led_state = 0;
                    gpio_set_level(LED_PIN, 0);
                    send_response("[OK] Variables reiniciadas\r\n");
                    break;

                default:
                    send_response("[ERROR] Comando invalido\r\n");
                    break;
            }
        }
    }
}

//tarea sensor
void task_sensor(void *pvParameters)
{
    while (1) {
        sensor_value += 5;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//tarea monitor
void task_monitor(void *pvParameters)
{
    char msg[100];

    while (1) {
        snprintf(msg, sizeof(msg),
                 "[MONITOR] LED=%s | Sensor=%d\r\n",
                 led_state ? "ON" : "OFF",
                 sensor_value);
        send_response(msg);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void app_main(void)
{
    uart2_init(UART_BAUD_RATE);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    command_queue = xQueueCreate(5, sizeof(command_t));

    xTaskCreate(task_uart_rx, "UART_RX", 4096, NULL, 3, NULL);
    xTaskCreate(task_control, "CONTROL", 2048, NULL, 2, NULL);
    xTaskCreate(task_sensor, "SENSOR", 2048, NULL, 1, NULL);
    xTaskCreate(task_monitor, "MONITOR", 2048, NULL, 1, NULL);
}
