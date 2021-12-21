#include "pages/lv_page.h"
#include "pages/lv_pages_include.h"

static lv_page_t main_page = {
    "main", NULL, lv_page_main_construct, NULL
};

void lv_xwatch_main(void)
{
    main_page.construct(&main_page);
}
