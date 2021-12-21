#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_files_construct(lv_page_t *files_page)
{
    lv_obj_t *files = NULL;

    files = lv_obj_create(lv_scr_act());
    files_page->obj = files;

    lv_obj_set_size(files, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(files, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(files);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(files_page);
    return 0;
}