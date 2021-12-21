#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_bluetooth_construct(lv_page_t *bluetooth_page)
{
    lv_obj_t *bluetooth = NULL;

    bluetooth = lv_obj_create(lv_scr_act());
    bluetooth_page->obj = bluetooth;

    lv_obj_set_size(bluetooth, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(bluetooth, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(bluetooth);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(bluetooth_page);
    return 0;
}