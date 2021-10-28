#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "unity.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_net.h"
#include "esp_log.h"

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
    if (argc < 3) {
        ESP_LOGE("cmd", "input args num: %d\n", argc);
        return -1;
    }

    app_net_start((unsigned char *)argv[1], (unsigned char *)argv[2]);
    return 0;
}

static esp_console_cmd_t usr_cmd[] = {
    {"test",        "This is test cmd",         NULL,   cmd_test,           NULL},
    {"wifi",        "wifi connect",             NULL,   cmd_wifi_config,    NULL},
};

int app_console_init(void)
{
    int ret;
    int i;
    esp_console_repl_config_t repl_config = {
        .max_history_len = 32,            \
        .history_save_path = NULL,        \
        .task_stack_size = 1024,          \
        .task_priority = 10,               \
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
        printf("console init failed\n");
        return -1;
    }

    for (i = 0; i < sizeof(usr_cmd) / sizeof(esp_console_cmd_t); i++) {
        printf("cmd[ %s ] register %s\n", usr_cmd[i].command, esp_console_cmd_register(&usr_cmd[i]) ? "failed" : "success");
    }

    ret = esp_console_start_repl(s_repl);
    if (ret) {
        printf("failed start repl\n");
        return -1;
    }

    printf("console init success\n");
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