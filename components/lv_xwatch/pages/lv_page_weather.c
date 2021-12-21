#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_weather_construct(lv_page_t *weather_page)
{
    lv_obj_t *weather = NULL;

    weather = lv_obj_create(lv_scr_act());
    weather_page->obj = weather;

    lv_obj_set_size(weather, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(weather, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(weather);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(weather_page);
    return 0;
}