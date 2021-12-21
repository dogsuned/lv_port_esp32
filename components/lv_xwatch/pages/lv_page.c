#include "lv_page.h"

static void lv_page_destruct(lv_page_t *page)
{
    if (page == NULL || page->obj == NULL) {
        // printf("null page\n");
        return;
    }

    lv_obj_del(page->obj);
}

static void lv_page_event_cb(lv_event_t * e)
{
    static lv_obj_t *last = NULL;
    lv_page_t *sub_page;
    lv_dir_t dir;

    sub_page = (lv_page_t *)e->user_data;
    switch (e->code) {
        case LV_EVENT_GESTURE:
            if (LV_DIR_RIGHT == lv_indev_get_gesture_dir(lv_indev_get_act())) {
                if (sub_page->parent->obj && lv_obj_has_flag(sub_page->parent->obj, LV_OBJ_FLAG_HIDDEN)) {
                    lv_indev_wait_release(lv_indev_get_act());
                    lv_obj_clear_flag(sub_page->parent->obj, LV_OBJ_FLAG_HIDDEN);
                    lv_page_destruct(sub_page);
                }
            }
            break;

        case LV_EVENT_CLICKED:
            if (lv_obj_has_flag(sub_page->parent->obj, LV_OBJ_FLAG_HIDDEN) == false) {
                lv_obj_add_flag(sub_page->parent->obj, LV_OBJ_FLAG_HIDDEN);
                sub_page->construct(sub_page);
            }
            break;

        default:
            break;
    }
}

lv_page_t *lv_page_get_by_name(lv_page_t *p, int size, char *name)
{
    int idx = 0;

    if (p == NULL || name == NULL) {
        return NULL;
    }

    while (idx < size) {
        if (strcmp(p->name, name) == 0) {
            return p;
        }
        p++;
    }

    return NULL;
}

void lv_page_attach_element(lv_page_t *main, lv_page_t *sub, lv_obj_t *element)
{
    if (main == NULL || sub == NULL || element == NULL) {
        return;
    }

    sub->parent = main;
    lv_obj_add_event_cb(element, lv_page_event_cb, LV_EVENT_CLICKED, (void *)sub);
    lv_obj_add_flag(element, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(element, LV_OBJ_FLAG_CLICKABLE);
}

void lv_page_add_gesture(lv_page_t *sub)
{
    if (sub == NULL) {
        return;
    }

    lv_obj_add_event_cb(sub->obj, lv_page_event_cb, LV_EVENT_GESTURE, (void *)sub);
    lv_obj_clear_flag(sub->obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(sub->obj, LV_OBJ_FLAG_CLICKABLE);
}