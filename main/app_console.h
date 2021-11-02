#ifndef _APP_CONSOLE_H
#define _APP_CONSOLE_H

#include "esp_console.h"

char *console_get_string(void);
esp_err_t app_console_cmd_register(const char *cmd, esp_console_cmd_func_t cmd_func);
int app_console_init(void);
int app_console_deinit(void);
#endif