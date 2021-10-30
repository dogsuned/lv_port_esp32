#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "unity.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_net.h"
#include "esp_log.h"
#include "app_console.h"
#include "app_iot.h"

static const char* TAG = "console";

typedef struct {
    const char *cmd;
    esp_console_cmd_func_t func;
} console_cmd_t;

static esp_console_repl_t *s_repl = NULL;

static int cmd_test(int argc, char **argv)
{
    int i = 0;

    while (argc > i) {
        printf("arg[%d] -----> %s\n", i, argv[i]);
        i++;
    }

    return 0;
}

static int cmd_wifi_config(int argc, char **argv)
{
#if 0
    if (argc < 3) {
        ESP_LOGE("cmd", "input args num: %d\n", argc);
        return -1;
    }

    app_net_start((unsigned char *)argv[1], (unsigned char *)argv[2]);
#else
    app_net_start((unsigned char *)"TP-LINK_5487", (unsigned char *)"6302ABCD!");
#endif

    return 0;
}

static int cmd_mqtt_start(int argc, char **argv)
{
    app_iot_start();
    return 0;
}

static console_cmd_t usr_cmd[] = {
    {"test",    cmd_test},
    {"wifi",    cmd_wifi_config},
    {"mqtt",    cmd_mqtt_start},
};

esp_err_t app_console_cmd_register(const char *cmd, esp_console_cmd_func_t cmd_func)
{
    esp_console_cmd_t cmd_config;
    esp_err_t retval;

    if (cmd == NULL || cmd_func == NULL) {
        ESP_LOGE(TAG, "invalid args\n");
        return -1;
    }

    memset(&cmd_config, 0, sizeof(esp_console_cmd_t));
    cmd_config.command = cmd;
    cmd_config.func = cmd_func;

    retval = esp_console_cmd_register(&cmd_config);
    ESP_LOGI(TAG, "console cmd: %s register %s\n", cmd, retval ? "failed" : "success");

    return retval;
}

int app_console_init(void)
{
    int ret;
    int i;
    esp_console_repl_config_t repl_config = {
        .max_history_len = 32,            \
        .history_save_path = NULL,        \
        .task_stack_size = 4096,          \
        .task_priority = 10,              \
        .prompt = NULL,                   \
        .max_cmdline_length = 0
    };

    esp_console_dev_uart_config_t uart_config = {
        .channel = CONFIG_ESP_CONSOLE_UART_NUM,         \
        .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,  \
        .tx_gpio_num = -1, \
        .rx_gpio_num = -1,
    };

    ret = esp_console_new_repl_uart(&uart_config, &repl_config, &s_repl);
    if (ret) {
        ESP_LOGE(TAG, "console init failed\n");
        return -1;
    }

    for (i = 0; i < sizeof(usr_cmd) / sizeof(console_cmd_t); i++) {
        app_console_cmd_register(usr_cmd[i].cmd, usr_cmd[i].func);
    }

    ret = esp_console_start_repl(s_repl);
    if (ret) {
        ESP_LOGE(TAG, "failed start repl\n");
        return -1;
    }

    ESP_LOGI(TAG, "console init success\n");
    return 0;
}

int app_console_deinit(void)
{
    if (s_repl == NULL) {
        printf("console has not been init\n");
        return 0;
    }

    return s_repl->del(s_repl);
}