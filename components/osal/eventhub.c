#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "eventhub.h"
#include <stdbool.h>
#include <string.h>
#include "esp_log.h"

#define EVENTHUB_NODE_MAX           (8)
#define EVENTHUB_NODE_UNUSED_ID     (0)

typedef struct {
    int id;
    eventhub_callback_f cb;
} eventhub_node_t;

typedef struct {
    bool inited;
    EventGroupHandle_t eventgroup;
    eventhub_node_t node[EVENTHUB_NODE_MAX];
} eventhub_ctx_t;

static eventhub_ctx_t g_eventhub_ctx = {
    .inited = false,
};
static char *TAG = "eventhub";

static eventhub_node_t *eventhub_get_unused_node(void)
{
    int idx;

    if (g_eventhub_ctx.inited == false) {
        return NULL;
    }

    for (idx = 0; idx < EVENTHUB_NODE_MAX; idx++) {
        if (g_eventhub_ctx.node[idx].id == EVENTHUB_NODE_UNUSED_ID) {
            g_eventhub_ctx.node[idx].id = idx + 1;
            return &g_eventhub_ctx.node[idx];
        }
    }

    return NULL;
}

static void eventhub_handle_task(void *arg)
{
    EventBits_t event;
    int idx;

    while (1) {
        event = xEventGroupWaitBits(g_eventhub_ctx.eventgroup, 0x00FFFFFF, false, false, portMAX_DELAY);
        for (idx = 0; idx < EVENTHUB_NODE_MAX; idx++) {
            if (g_eventhub_ctx.node[idx].id != EVENTHUB_NODE_UNUSED_ID) {
                g_eventhub_ctx.node[idx].cb ? g_eventhub_ctx.node[idx].cb(event) : 0;
                xEventGroupClearBits(g_eventhub_ctx.eventgroup, event);
            }
        }
    }
}

int eventhub_subscribe(eventhub_callback_f cb)
{
    eventhub_node_t *node = NULL;

    if (g_eventhub_ctx.inited == false) {
        ESP_LOGE(TAG, "eventhub has not been inited\n");
        return -1;
    }

    if (cb == NULL) {
        ESP_LOGW(TAG, "make sure event callback exist\n");
        return -1;
    }

    node = eventhub_get_unused_node();
    if (node == NULL) {
        ESP_LOGE(TAG, "eventhub node may exceed max count: %d\n", EVENTHUB_NODE_MAX);
        return -1;
    }

    node->cb = cb;
    return 0;
}

int eventhub_publish(int type, int from_isr)
{
    BaseType_t xHigherPriorityTaskWoken = 0;

    if (g_eventhub_ctx.inited == false) {
        ESP_LOGE(TAG, "eventhub has not been inited\n");
        return -1;
    }

    if (from_isr) {
        xEventGroupSetBitsFromISR(g_eventhub_ctx.eventgroup, type, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else {
        xEventGroupSetBits(g_eventhub_ctx.eventgroup, type);
    }

    return 0;
}

int eventhub_init(void)
{
    int ret;

    if (g_eventhub_ctx.inited == true) {
        ESP_LOGW(TAG, "eventhub module already inited\n");
        return 0;
    }

    memset((void *)&g_eventhub_ctx, 0, sizeof(eventhub_ctx_t));
    g_eventhub_ctx.eventgroup = xEventGroupCreate();
    if (g_eventhub_ctx.eventgroup == NULL) {
        ESP_LOGE(TAG, "failed create eventgroup\n");
        return -1;
    }

    ret = xTaskCreate(eventhub_handle_task, "eventhub", 2 * 1024, NULL, 5, NULL);
    if (ret != pdTRUE) {
        ESP_LOGE(TAG, "failed create eventhub task, ret: %d\n", ret);
        vEventGroupDelete(g_eventhub_ctx.eventgroup);
        return -1;
    }

    g_eventhub_ctx.inited = true;
    ESP_LOGI(TAG, "eventhub init success\n");

    return 0;
}