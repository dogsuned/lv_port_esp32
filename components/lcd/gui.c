#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "esp_heap_caps.h"
#include "gui.h"
#include "font.h"

#define CANVAS_ROW  320
#define CANVAS_ROL  320
#define FLUSH_MAX_LINES 30

#define CHECK_NULL(x)   do {if (x == NULL) {\
    printf("input null, line: %d\n", __LINE__);\
    return -1;\
}} while (0);

const unsigned short g_color[GUI_COLOR_MAX] = { 
    0x0000, 0xFFFF, 0x0000, 0x1F00, 0X1FF8, 
    0XFF07, 0x00F8, 0x1FF8, 0xE007, 0xFF7F, 
    0xE0FF, 0X40BC, 0X07FC, 0X3084, 0XCF01, 
    0X7C7D, 0X5854, 0X1F84, 0X5BEF, 0X18C6, 
    0X51A6, 0X122B
};
static guiobj_t *g_gui = NULL;

static inline void swap(ushort *a, ushort *b)
{
    ushort temp = *a;
    *a = *b;
    *b = temp;
}

static int pixel_convert_color(uint xs, uint ys, uint wx, uint wy)
{
    uint row, col;
    color_index_e color;

    CHECK_NULL(g_gui->canvas.pixel);
    CHECK_NULL(g_gui->flush_buf);

    if (xs + wx > g_gui->canvas.col || ys + wy > g_gui->canvas.row) {
        printf("coordinate exceed canvas\n");
        return -1;
    }

    for (row = 0; row < wy; row++) {
        for (col = 0; col < wx; col++) {
            color = (color_index_e)g_gui->canvas.pixel[ys + row][xs + col];
            if (color == GUI_AUTO) {
                color = g_gui->settings.fg;
            }
            g_gui->flush_buf[row * wx + col] = g_color[color];
        }
    }

    return 0;
}

static int flush(void)
{
    int row;
    int deltay;
    int wry;
    coordinate_t coord = {
        .xs = 0,
        .ys = 0,
        .xe = g_gui->canvas.col,
        .ye = g_gui->canvas.row
    };

    CHECK_NULL(g_gui->flush_buf);
    CHECK_NULL(g_gui->lcd_flush);

    deltay = coord.ye - coord.ys;
    row = coord.ys;

    while (row < coord.ye) {
        wry = deltay > FLUSH_MAX_LINES ? FLUSH_MAX_LINES : deltay;
        if (pixel_convert_color(coord.xs, row, coord.xe - coord.xs, wry)) {
            printf("convert failed row: %d, wx: %d, wy: %d\n", row, coord.xe - coord.xs, wry);
            return -1;
        }

        if (g_gui->lcd_flush(coord.xs, row, coord.xe, row + wry, g_gui->flush_buf)) {
            printf("lcd flush failed\n");
			return -1;
        }
        row += wry;
        deltay -= wry;
    }

    return 0;
}

static int draw_line(ushort xs, ushort ys, ushort xe, ushort ye, ushort size, color_index_e color)
{
    int t;
    int xerr=0, yerr=0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    delta_x = xe - xs;
    delta_y = ye - ys;
    row = ys;
    col = xs;

    if (delta_x > 0)
        incx = 1;
    else if (delta_x == 0)
        incx = 0;
    else {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;
    else {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)
        distance = delta_x;
    else
        distance = delta_y;

    for(t = 0; t <= distance + 1; t++ ) {
        if (row < g_gui->canvas.row && col < g_gui->canvas.col) {
            g_gui->canvas.pixel[row][col] = color;
        }

        xerr += delta_x;
        yerr += delta_y;

        if(xerr > distance) { 
            xerr -= distance; 
            col += incx; 
        }

        if(yerr > distance) { 
            yerr -= distance;
            row += incy;
        }
    }

    return 0;
}

static int draw_rectangle(ushort xs, ushort ys, ushort xe, ushort ye, ushort size, color_index_e color)
{
    draw_line(xs, ys, xe, ys, size, color);
	draw_line(xs, ys, xs, ye, size, color);
	draw_line(xs, ye, xe, ye, size, color);
	draw_line(xe, ys, xe, ye, size, color);

    return 0;
}

static void _draw_circle(int xc, int yc, int x, int y, color_index_e color)
{
    ushort xmax, ymax;

    xmax = g_gui->canvas.col;
    ymax = g_gui->canvas.row;

    if ((yc + y) >= 0 && (yc + y) < ymax) {
        if ((xc + x) >= 0 && (xc + x) < xmax)
            g_gui->canvas.pixel[yc + y][xc + x] = color;
        if ((xc - x) >= 0 && (xc - x) < xmax)
            g_gui->canvas.pixel[yc + y][xc - x] = color;
    }
    if ((yc - y) >= 0 && (yc - y) < ymax) {
        if ((xc + x) >= 0 && (xc + x) < xmax)
            g_gui->canvas.pixel[yc - y][xc + x] = color;
        if ((xc - x) >= 0 && (xc - x) < xmax)
            g_gui->canvas.pixel[yc - y][xc - x] = color;
    }

    if ((yc + x) >= 0 && (yc + x) < ymax) {
        if ((xc + y) >= 0 && (xc + y) < xmax)
            g_gui->canvas.pixel[yc + x][xc + y] = color;
        if ((xc - y) >= 0 && (xc - y) < xmax)
            g_gui->canvas.pixel[yc + x][xc - y] = color;
    }
    if ((yc - x) >= 0 && (yc - x) < ymax) {
        if ((xc + y) >= 0 && (xc + y) < xmax)
            g_gui->canvas.pixel[yc - x][xc + y] = color;
        if ((xc - y) >= 0 && (xc - y) < xmax)
            g_gui->canvas.pixel[yc - x][xc - y] = color;
    }  
}

static int draw_circle(ushort xc, ushort yc, ushort r, color_index_e color, int fill)
{
    int x = 0, y = r, yi, d;
	d = 3 - 2 * r;

    while (x <= y) {
        if (fill) {
            for (yi = x; yi <= y; yi++)
                _draw_circle(xc, yc, x, yi, color);
        } else {
            _draw_circle(xc, yc, x, y, color);
        }

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    return 0;
}

static char *get_font_data(char c, int size)
{
    char *p = NULL;

    if (c < ' ' || c > '~') {
        return NULL;
    }

    switch (size) {
        case 16:
            p = (char *)ascii_font_0816[c - ' '];
            break;
        case 32:
            p = (char *)ascii_font_1632[c - ' '];
            break;
        case 48:
            p = (char *)ascii_font_2448[c - ' '];
            break;
        default:
            printf("unsupport font size: %d\n", size);
            break;
    }

    return p;
}

static int show_string(ushort x, ushort y, ushort size, char *p, ushort len, color_index_e color)
{
    ushort i, j, row, col;
    char *data;

    CHECK_NULL(p);

    if (size % 8 || size < 16 || size > 48) {
        printf("only support font size(16 32 48)\n");
        return -1;
    }

    for (i = 0; i < len; i++) {
        if ((data = get_font_data(p[i], size)) == NULL) {
            printf("unrecorgnize %c\n", p[i]);
            continue;
        }
        for (row = 0; row < size; row++) {
            if (y + row >= g_gui->canvas.row) {
                break;
            }
            for (col = 0; col < size / 16; col++) {
                if (x + col * 8 >= g_gui->canvas.col) {
                    break;
                }
                for (j = 0; j < 8; j++) {
                    if (x + col * 8 + j >= g_gui->canvas.col) {
                        break;
                    }
                    if ((data[row * size / 16 + col] >> j) & 0x1) {
                        g_gui->canvas.pixel[y + row][x + col * 8 + j] = color;
                    } else {
                        g_gui->canvas.pixel[y + row][x + col * 8 + j] = g_gui->settings.bg;
                    }
                }
            }
        }
        x += size / 2;
    }

    return 0;
}

static int draw_bitmap(ushort xs, ushort ys, ushort xe, ushort ye, ushort *pcolor)
{
    int row = 0;
    int wry;
    int max_line;
    int deltax;

    CHECK_NULL(pcolor);

    if (xs == xe || ys == ye) {
        printf("width null\n");
        return -1;
    }

    if (xs > xe) {
        swap(&xs, &xe);
    }

    if (ys > ye) {
        swap(&ys, &ye);
    }

    deltax = xe - xs;
    max_line = FLUSH_MAX_LINES * g_gui->canvas.col / deltax;
    max_line > FLUSH_MAX_LINES ? max_line = FLUSH_MAX_LINES : 0;

    while (row < ye) {
        wry = ye - row > max_line ? max_line : ye - row;
        memcpy(g_gui->flush_buf, pcolor, wry * deltax * sizeof(ushort));

        if (g_gui->lcd_flush(xs, row, xe, row + wry, g_gui->flush_buf)) {
            printf("lcd flush failed\n");
			return -1;
        }
        row += wry;
        pcolor += wry * deltax;
    }

    return 0;
}

static int fill(ushort xs, ushort ys, ushort xe, ushort ye, color_index_e color)
{
    uint row;

    if (xs > xe) {
        swap(&xs, &xe);
    }

    if (ys > ye) {
        swap(&ys, &ye);
    }

    if (xe > g_gui->canvas.col) {
        xe = g_gui->canvas.col;
    }

    if (ye > g_gui->canvas.row) {
        ye = g_gui->canvas.row;
    }

    for (row = ys; row < ye; row++) {
        memset(g_gui->canvas.pixel[row], color == GUI_AUTO ? g_gui->settings.bg : color, xe - xs);
    }

    return 0;
}

static int clear(void)
{
    return fill(0, 0, g_gui->canvas.col, g_gui->canvas.row, g_gui->settings.bg);
}

static int canvas_deinit(canvas_t *canvas)
{
    if (canvas == NULL || canvas->pixel == NULL) {
        return -1;
    }

    while(*(canvas->pixel)) {
        free(*(canvas->pixel));
        *(canvas->pixel) = NULL;
        canvas->pixel++;
    }

    canvas->row = 0;
    canvas->col = 0;
    canvas->pixel = NULL;
    printf("canvas deinit success\n");
    return 0;
}

static int canvas_init(canvas_t *canvas, unsigned int row, unsigned int col)
{
    int i;

    if (canvas->pixel) {
        printf("canvas already inited\n");
        return 0;
    }

    canvas->pixel = calloc(row * sizeof(char *) + 1, 1);
    if (canvas->pixel == NULL) {
        printf("canvas malloc failed\n");
        goto canvas_err;
    }

    for (i = 0; i < row; i++) {
        canvas->pixel[i] = (char *)malloc(col);
        if (canvas->pixel[i] == NULL) {
            printf("canvas row[%d] malloc failed\n", i);
            goto canvas_err;
        }
    }

    canvas->row = row;
    canvas->col = col;
    printf("canvas init success\n");
    return 0;

canvas_err:
    canvas_deinit(canvas);
    return -1;
}

static int gui_destructor(guiobj_t *gui)
{
    if (gui == NULL) {
        printf("gui null\n");
        return 0;
    }

    /* delete canvas */
    canvas_deinit(&gui->canvas);

    /* free buffer */
    heap_caps_free(g_gui->flush_buf);

    free(gui);
    gui = NULL;

    return 0;
}

static guiobj_t *gui_constructor(void)
{
    guiobj_t *gui = NULL;

    gui = (guiobj_t *)calloc(sizeof(guiobj_t), 1);
    if (gui == NULL) {
        return NULL;
    }

    gui->destructor = gui_destructor;
    return gui;
}

static int gui_start(lcd_flush_f lcd_flush, color_index_e fg, color_index_e bg)
{
    g_gui->lcd_flush = lcd_flush;
    g_gui->settings.fg = fg;
    g_gui->settings.bg = bg;

    g_gui->ops.clear();
    g_gui->ops.flush();

    return 0;
}

static gui_ops_t gui_ops = {
    .draw_line = draw_line,
    .draw_rectangle = draw_rectangle,
    .draw_circle = draw_circle,
    .show_string = show_string,
    .draw_bitmap = draw_bitmap,
    .fill = fill,
    .clear = clear,
    .flush = flush,
};

guiobj_t *gui_create(uint xpixel, uint ypixel)
{
    int ret;

    g_gui = gui_constructor();
    if (g_gui == NULL) {
        printf("gui constructor failed\n");
        goto gui_err;
    }

    ret = canvas_init(&g_gui->canvas, ypixel, xpixel);
    if (ret) {
        printf("failed create canvas\n");
        goto gui_err;
    }

    g_gui->flush_buf = heap_caps_malloc(xpixel * FLUSH_MAX_LINES * sizeof(uint16_t), MALLOC_CAP_DMA);
    if (g_gui->flush_buf == NULL) {
        printf("failed malloc flush buffer\n");
        goto gui_err;
    }

    g_gui->ops = gui_ops;
    g_gui->gui_start = gui_start;

    printf("gui create success\n");
    return g_gui;

gui_err:
    if (g_gui) {
        g_gui->destructor(g_gui);
    }
    return NULL;
}