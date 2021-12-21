#include "lv_page.h"
#include "lv_pages_include.h"

#if 0
void analog( lv_obj_t *win)
{
    lv_obj_t* central = win; 

    lv_obj_set_size(central, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(central,  0, 0);

    lv_obj_t * img = lv_img_create(central,NULL);
    lv_img_set_src(img, &watch_bg);  //加载表盘

    lv_obj_set_size(img, 200, 200);
    lv_obj_set_auto_realign(img, true);
    lv_obj_align(img, central, LV_ALIGN_CENTER, 0, 0);

    lvHour = lv_img_create(central,NULL);  // 时针
    lv_img_set_src(lvHour, &hour);
    lv_obj_align(lvHour, img,LV_ALIGN_CENTER, 0, 0);
    uint16_t h = Hour * 300 + Minute / 12 % 12 * 60;
    lv_img_set_angle(  lvHour, h);

    lvMinute = lv_img_create(central,NULL);  // 分针
    lv_img_set_src( lvMinute, &minute);
    lv_obj_align(  lvMinute, img,LV_ALIGN_CENTER, 0, 0);
    lv_img_set_angle(  lvHour, Minute*60);

    lvSecond = lv_img_create(central,NULL);  //秒针

    lv_img_set_src( lvSecond, &second);
    lv_obj_align(  lvSecond, img,LV_ALIGN_CENTER, 0, 0);
    lv_img_set_angle(  lvSecond, Second*60);
	
     lv_task_create(update_time, 1000, LV_TASK_PRIO_LOW, NULL);  // 1秒任务
}

static void update_time(void *arg)
{
    if(lvHour !=NULL) {
       // Hour,Minute,Second;
        uint16_t h = Hour * 300 + Minute / 12 % 12 * 60;
        lv_img_set_angle(lvHour, h);
        lv_img_set_angle(lvMinute, Minute * 6 * 10);
       
        lv_img_set_angle(lvSecond, Second * 6 * 10);
        printf("h %d m %d s %d\n", Hour, Minute, Second);
    }

    if (++Second >= 60) {
        Second = 0;
        if (++Minute >= 60) {
            Minute = 0;
            if (++Hour >= 12) Hour = 0;
        }

    }
}
#endif

int lv_page_clock_construct(lv_page_t *clock_page)
{
    lv_obj_t *clock = NULL;

    clock = lv_obj_create(lv_scr_act());
    clock_page->obj = clock;

    lv_obj_set_size(clock, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(clock, lv_color_hex(LV_SYSTEM_BG_COLOR), 0);

    lv_obj_t *label = lv_label_create(clock);
    lv_label_set_text(label, __func__);

    lv_page_add_gesture(clock_page);
    return 0;
}