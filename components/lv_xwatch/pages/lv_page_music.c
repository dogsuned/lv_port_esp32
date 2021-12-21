#include "lv_page.h"
#include "lv_pages_include.h"

int lv_page_music_construct(lv_page_t *music_page)
{
    lv_obj_t *music = NULL;

    music = lv_obj_create(lv_scr_act());
    music_page->obj = music;

    lv_obj_set_size(music, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(music, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(music);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(music_page);
    return 0;
}