#include <stdlib.h>
#include <stdbool.h>
#include <esp_system.h>
#include <esp_http_server.h>

#define HTTPD_TEST_MAX_URI_HANDLERS 3

esp_err_t null_func(httpd_req_t *req)
{
    printf("------------ http success, uri: %s, method: %d-----------\n", req->uri, req->method);
    return ESP_OK;
}

httpd_uri_t uris[HTTPD_TEST_MAX_URI_HANDLERS] = {
    {
        .uri      = "/test",
        .method   = HTTP_GET,
        .handler  = null_func,
        .user_ctx = NULL,
    },
};

void app_http_server_start(void)
{
    httpd_handle_t hd;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = HTTPD_TEST_MAX_URI_HANDLERS;
    printf("-------- start: %d --------\n", httpd_start(&hd, &config));
    printf("-------- uri: %d --------\n", httpd_register_uri_handler(hd, &uris[0]));
}