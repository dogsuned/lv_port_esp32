#include "lv_page.h"
#include "lv_pages_include.h"

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t date;
        if(lv_calendar_get_pressed_date(obj, &date)) {
            LV_LOG_USER("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
        }
    }
}

static void btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt = lv_btnmatrix_get_btn_text(obj, id);

        LV_LOG_USER("%s was pressed\n", txt);
    }
}


static const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                                  "6", "7", "8", "9", "0", "\n",
                                  "Action1", "Action2", ""};

void lv_example_btnmatrix_my(lv_obj_t *parent)
{
    lv_obj_t * btnm1 = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(btnm1, btnm_map);
    lv_obj_align(btnm1, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(btnm1, btn_event_handler, LV_EVENT_ALL, NULL);
}

static void arrow_event_handler(lv_event_t * e)
{
    if (e->code == LV_EVENT_CLICKED) {
        lv_example_btnmatrix_my((lv_obj_t *)e->user_data);
    }
}

void lv_example_calendar_my(lv_obj_t *parent)
{
    lv_obj_t *arrow;
    lv_obj_t  *calendar = lv_calendar_create(parent);
    lv_obj_set_size(calendar, lv_pct(100), lv_pct(100));
    lv_obj_align(calendar, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

    lv_calendar_set_today_date(calendar, 2021, 12, 4);
    lv_calendar_set_showed_date(calendar, 2021, 12);

    // /*Highlight a few days*/
    // static lv_calendar_date_t highlighted_days[1];
    // highlighted_days[0].year = 2021;
    // highlighted_days[0].month = 12;
    // highlighted_days[0].day = 7;

    // lv_calendar_set_highlighted_dates(calendar, highlighted_days, 1);

    arrow = lv_calendar_header_arrow_create(parent, calendar, 25);
    lv_obj_add_event_cb(arrow, arrow_event_handler, LV_EVENT_CLICKED, (void *)calendar);
}

int lv_page_calendar_construct(lv_page_t *calendar_page)
{
    lv_obj_t *calendar = NULL;

    calendar = lv_obj_create(lv_scr_act());
    calendar_page->obj = calendar;

    lv_obj_set_size(calendar, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_all(calendar, 0, 0);
    lv_obj_set_style_bg_color(calendar, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_page_add_gesture(calendar_page);
    lv_example_calendar_my(calendar);
    return 0;
}