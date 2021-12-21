#include "lv_page.h"
#include "lv_pages_include.h"

LV_IMG_DECLARE(img_main_desktop_star);

static lv_page_t main_sub_pages[] = {
    {"menu", NULL, lv_page_menu_construct, NULL},
};

static lv_obj_t *create_title_bar(lv_obj_t *parent)
{
    static lv_style_t common_style;
    lv_obj_t *bell, *volume, *wifi, *battery, *bluetooth;

    lv_style_init(&common_style);
    lv_style_set_opa(&common_style, LV_OPA_100);

    /*Create the titles*/
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_height(cont, 15);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    bell = lv_label_create(cont);
    lv_obj_add_style(bell, &common_style, 0);
    lv_label_set_text(bell, LV_SYMBOL_BELL);

    bluetooth = lv_label_create(cont);
    lv_obj_add_style(bluetooth, &common_style, 0);
    lv_label_set_text(bluetooth, LV_SYMBOL_BLUETOOTH);

    volume = lv_label_create(cont);
    lv_obj_add_style(volume, &common_style, 0);
    lv_label_set_text(volume, LV_SYMBOL_VOLUME_MID);

    wifi = lv_label_create(cont);
    lv_obj_add_style(wifi, &common_style, 0);
    lv_label_set_text(wifi, LV_SYMBOL_WIFI);

    battery = lv_label_create(cont);
    lv_obj_add_style(battery, &common_style, 0);
    lv_label_set_text(battery, LV_SYMBOL_BATTERY_3);
    return cont;
}

static lv_obj_t *create_time_label(lv_obj_t *parent)
{
    static lv_style_t common_style;
    lv_obj_t *time_label, *date_label;

    lv_style_init(&common_style);
    lv_style_set_opa(&common_style, LV_OPA_100);
    lv_style_set_pad_column(&common_style, 0);
    lv_style_set_pad_row(&common_style, 0);

    /*Create the titles*/
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_height(cont, 90);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);

    time_label = lv_label_create(cont);
    lv_obj_add_style(time_label, &common_style, 0);
    lv_obj_set_style_text_font(time_label, (const lv_font_t *)&font_asc_chiller_72, 0);
    // lv_label_set_text_fmt(time_label, "%02d:%02d", hour, minute);
    lv_label_set_text(time_label, "17:51");

    date_label = lv_label_create(cont);
    lv_obj_add_style(date_label, &common_style, 0);
    lv_obj_set_style_text_font(date_label, (const lv_font_t *)&font_ch_xinwei_14, 0);
    // lv_label_set_text_fmt(date_label, "%04d.%02d.%02d %s", year, month, day, get_wday_str());
    lv_label_set_text(date_label, "2021.12.04 星期六");

    return cont;
}

static void lv_page_main(lv_page_t *main_page)
{
    lv_page_t *page;
    lv_obj_t *title, *time, *date;
    lv_anim_t *a;
    static const lv_coord_t grid_cols[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t grid_rows[] = {15, /* title */
                                           10, /* res */
                                           90, /* time & date*/
                                           125, /* res */
                                           LV_GRID_TEMPLATE_LAST};

    lv_obj_set_size(main_page->obj, 240, 240);
    lv_obj_align(main_page->obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_opa(main_page->obj, LV_OPA_60, 0);
    lv_img_set_src(main_page->obj, &img_main_desktop_star);

    lv_obj_t *con = lv_obj_create(main_page->obj);
    lv_obj_set_size(con, lv_pct(100), lv_pct(100));
    // lv_obj_clear_flag(con, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_opa(con, LV_OPA_0, 0);
    lv_obj_set_pos(con, 0, 0);
    lv_obj_set_style_pad_all(con, 0, 0);
    lv_obj_set_grid_dsc_array(con, grid_cols, grid_rows);
    lv_obj_set_style_grid_row_align(con, LV_GRID_ALIGN_START, 0);

    /* click to menu page */
    page = lv_page_get_by_name(main_sub_pages, sizeof(main_sub_pages) / sizeof(lv_page_t), "menu");
    lv_page_attach_element(main_page, page, con);

    title = create_title_bar(con);
    time = create_time_label(con);
    lv_page_attach_element(main_page, page, time);

    lv_obj_set_grid_cell(title, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(time, LV_GRID_ALIGN_CENTER, 0, 1, LV_ALIGN_CENTER, 2, 1);
}

int lv_page_main_construct(lv_page_t *main_page)
{
    lv_obj_t *window = NULL;

    if (main_page == NULL) {
        return -1;
    }

    window = lv_img_create(lv_scr_act());
    main_page->obj = window;

    lv_page_main(main_page);
    return 0;
}
