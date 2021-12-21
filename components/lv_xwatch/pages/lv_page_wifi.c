#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_wifi_construct(lv_page_t *wifi_page)
{
    lv_obj_t *wifi = NULL;

    wifi = lv_obj_create(lv_scr_act());
    wifi_page->obj = wifi;

    lv_obj_set_size(wifi, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(wifi, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(wifi);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(wifi_page);
    return 0;
}