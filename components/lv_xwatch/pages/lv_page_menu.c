#include "lv_page.h"
#include "lv_pages_include.h"

LV_IMG_DECLARE(img_menu_logo_setting);
LV_IMG_DECLARE(img_menu_logo_file);
LV_IMG_DECLARE(img_menu_logo_music);
LV_IMG_DECLARE(img_menu_logo_clock);
LV_IMG_DECLARE(img_menu_logo_weather);
LV_IMG_DECLARE(img_menu_logo_calendar);

typedef struct {
    lv_obj_t *img;
    void *src;
} _menu_logo_t;

static lv_page_t menu_sub_pages[] = {
    {"files",       NULL, lv_page_files_construct, NULL},
    {"setting",     NULL, lv_page_settings_construct, NULL},
    {"weather",     NULL, lv_page_weather_construct, NULL},
    {"music",       NULL, lv_page_music_construct, NULL},
    {"clock",       NULL, lv_page_clock_construct, NULL},
    {"calendar",    NULL, lv_page_calendar_construct, NULL},
};

static _menu_logo_t menu_logo[] = {
    {NULL, &img_menu_logo_file},
    {NULL, &img_menu_logo_setting},
    {NULL, &img_menu_logo_weather},
    {NULL, &img_menu_logo_music},
    {NULL, &img_menu_logo_clock},
    {NULL, &img_menu_logo_calendar},
};

/* e.g.
 * btn = lv_btn_create(menu_page->obj);
 * label = lv_label_create(btn);
 * lv_label_set_text(label, "Button");
 * lv_obj_center(label)
 * page = lv_page_get_by_name(menu_sub_pages, sizeof(menu_sub_pages) / sizeof(lv_page_t), "setting");
 * lv_page_attach_element(menu_page, page, btn);
 */

static void lv_page_menu(lv_page_t *menu_page)
{
    int idx;
    static lv_style_t common_style;
    lv_page_t *page;
    lv_obj_t *btn, *label;

    lv_obj_set_pos(menu_page->obj, 0, 0);
    lv_obj_set_size(menu_page->obj, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(menu_page->obj, lv_color_hex(0), 0);

    lv_style_init(&common_style);
    lv_style_set_radius(&common_style, 0);
    lv_style_set_pad_row(&common_style, 15);
    lv_style_set_pad_column(&common_style, 15);
    lv_style_set_border_width(&common_style, 0);
    lv_style_set_bg_color(&common_style, lv_color_hex(0));

    lv_obj_t * cont = menu_page->obj;
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 10);
    lv_obj_add_style(cont, &common_style, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);

    for(idx = 0; idx < sizeof(menu_sub_pages) / sizeof(lv_page_t); idx++) {
        menu_logo[idx].img = lv_img_create(cont);
        lv_obj_set_size(menu_logo[idx].img, 60, 60);
        lv_obj_align(menu_logo[idx].img, LV_ALIGN_CENTER, 0, 0);
        lv_img_set_src(menu_logo[idx].img, menu_logo[idx].src);

        lv_page_attach_element(menu_page, &menu_sub_pages[idx], menu_logo[idx].img);
    }
}

int lv_page_menu_construct(lv_page_t *menu_page)
{
    lv_obj_t *menu = NULL;

    menu = lv_obj_create(lv_scr_act());
    menu_page->obj = menu;

    lv_page_add_gesture(menu_page);
    lv_page_menu(menu_page);
    return 0;
}
