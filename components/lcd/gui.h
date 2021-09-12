#ifndef __GUI_H
#define __GUI_H

#include "gui_types.h"

typedef enum {
    GUI_AUTO,
    GUI_WHITE,
    GUI_BLACK,
    GUI_BLUE,
    GUI_BRED,
    GUI_GBLUE,
    GUI_RED,
    GUI_MAGENTA,
    GUI_GREEN,
    GUI_CYAN,
    GUI_YELLOW,
    GUI_BROWN,
    GUI_BRRED,
    GUI_GRAY,
    GUI_DARKBLUE,
    GUI_LIGHTBLUE,
    GUI_GRAYBLUE,
    GUI_LIGHTGREEN,
    GUI_LIGHTGRAY,
    GUI_LGRAY,
    GUI_LGRAYBLUE,
    GUI_LBBLUE,
    GUI_COLOR_MAX
} color_index_e;

typedef struct _guiobj guiobj_t;
typedef struct {
    ushort xs;
    ushort ys;
    ushort xe;
    ushort ye;
} coordinate_t;

typedef struct {
    char **pixel;
    unsigned int row;
    unsigned int col;
} canvas_t;

typedef struct {
    color_index_e bg;
    color_index_e fg;
} gui_settings_t;

typedef struct {
    int (*draw_line)(ushort xs, ushort ys, ushort xe, ushort ye, ushort size, color_index_e color);
    int (*draw_rectangle)(ushort xs, ushort ys, ushort xe, ushort ye, ushort size, color_index_e color);
    int (*draw_circle)(ushort xc, ushort yc, ushort r, color_index_e color, int fill);
    int (*show_string)(ushort x, ushort y, ushort size, char *p, ushort len, color_index_e color);
    int (*fill)(ushort xs, ushort ys, ushort xe, ushort ye, color_index_e color);
    int (*draw_bitmap)(ushort xs, ushort ys, ushort xe, ushort ye, ushort *pcolor);
    int (*clear)(void);
    int (*flush)(void);
} gui_ops_t;

typedef int (*lcd_flush_f)(ushort xs, ushort ys, ushort xe, ushort ye, ushort *data);

struct _guiobj{
    canvas_t canvas;
    gui_ops_t ops;
    gui_settings_t settings;
    ushort *flush_buf;

    int (*gui_start)(lcd_flush_f lcd_flush, color_index_e fg, color_index_e bg);
    int (*lcd_flush)(ushort xs, ushort ys, ushort xe, ushort ye, ushort *data);
    int (*destructor)(guiobj_t *gui);
};

guiobj_t *gui_create(uint xpixel, uint ypixel);

#endif
