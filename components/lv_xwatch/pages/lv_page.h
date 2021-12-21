#ifndef __LV_PAGE_H
#define __LV_PAGE_H

#include "../../lvgl/lvgl.h"
#include "../assets/font/font.h"

typedef struct _lv_page_t lv_page_t;
typedef int (*lv_page_construct_f)(lv_page_t *parent);

struct _lv_page_t {
    char *name;
    lv_obj_t *obj;
    lv_page_construct_f construct;
    lv_page_t *parent;
};

/**
 * get sub page by page name
 * @param p         pointer to pages
 * @param size      pages num.
 * @param name      sub page name.
 * @return  On success, return page pointer. On error, return NULL.
 */
lv_page_t *lv_page_get_by_name(lv_page_t *p, int size, char *name);

/**
 * make page switch while element clicked
 * @param parent        pointer to main page.
 * @param sub           pointer to sub page.
 * @param element       main page element.
 */
void lv_page_attach_element(lv_page_t *parent, lv_page_t *sub, lv_obj_t *element);

/**
 * sub page add gesture, back to parent
 * @param sub           pointer to sub page.
 */
void lv_page_add_gesture(lv_page_t *sub);

#endif
