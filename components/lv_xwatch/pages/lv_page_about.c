#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_about_construct(lv_page_t *about_page)
{
    lv_obj_t *about = NULL;

    about = lv_obj_create(lv_scr_act());
    about_page->obj = about;

    lv_obj_set_size(about, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(about, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(about);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(about_page);
    return 0;
}