#include "lv_page.h"
#include "lv_pages_include.h"

static lv_obj_t *setting_list;
static lv_page_t setting_sub_pages[] = {
    {"wifi",        NULL, lv_page_wifi_construct, NULL},
    {"bluetooth",   NULL, lv_page_bluetooth_construct, NULL},
    {"volume",      NULL, lv_page_volume_construct, NULL},
    {"about",       NULL, lv_page_about_construct, NULL},
};
static char *settings_icon[] = {
    LV_SYMBOL_WIFI,
    LV_SYMBOL_BLUETOOTH,
    LV_SYMBOL_VOLUME_MAX,
    LV_SYMBOL_HOME
};

static void event_handler(lv_event_t * e)
{
    int idx;
    char *item_name = NULL;
    lv_page_t *sub_page;

    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        item_name = lv_list_get_btn_text(setting_list, lv_event_get_target(e));
        if (item_name) {
            sub_page = lv_page_get_by_name(setting_sub_pages, sizeof(setting_sub_pages) / sizeof(lv_page_t), item_name);
            if (sub_page) {
                sub_page->parent = (lv_page_t *)e->user_data;
                if (lv_obj_has_flag(sub_page->parent->obj, LV_OBJ_FLAG_HIDDEN) == false) {
                    lv_obj_add_flag(sub_page->parent->obj, LV_OBJ_FLAG_HIDDEN);
                    sub_page->construct(sub_page);
                }
            }
        }
    }
}

static void lv_page_settings(lv_page_t *setting_page)
{
    static lv_style_t list_style;
    int idx;
    lv_obj_t *btn;

    lv_obj_set_pos(setting_page->obj, 0, 0);
    lv_obj_set_size(setting_page->obj, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(setting_page->obj, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_set_style_pad_all(setting_page->obj, 0, 0);

    lv_style_init(&list_style);
    lv_style_set_radius(&list_style, 5);
    lv_style_set_pad_row(&list_style, 0);
    lv_style_set_pad_column(&list_style, 0);
    lv_style_set_border_width(&list_style, 0);

    setting_list = lv_list_create(setting_page->obj);
    lv_obj_set_size(setting_list, lv_pct(100), lv_pct(100));
    lv_obj_center(setting_list);
    lv_obj_add_style(setting_list, &list_style, 0);

    lv_list_add_text(setting_list, "Settings");
    for (idx = 0; idx < sizeof(setting_sub_pages) / sizeof(lv_page_t); idx++) {
        btn = lv_list_add_btn(setting_list, settings_icon[idx], setting_sub_pages[idx].name);
        lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void *)setting_page);
    }
}

int lv_page_settings_construct(lv_page_t *setting_page)
{
    lv_obj_t *settings = NULL;

    settings = lv_obj_create(lv_scr_act());
    setting_page->obj = settings;

    lv_page_add_gesture(setting_page);
    lv_page_settings(setting_page);
    return 0;
}