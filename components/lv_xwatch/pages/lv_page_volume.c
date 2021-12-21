#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_volume_construct(lv_page_t *volume_page)
{
    lv_obj_t *volume = NULL;

    volume = lv_obj_create(lv_scr_act());
    volume_page->obj = volume;

    lv_obj_set_size(volume, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(volume, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(volume);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(volume_page);
    return 0;
}