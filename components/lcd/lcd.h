#ifndef _LCD_H
#define _LCD_H

int lcd_init(void);
esp_err_t lcd_display_jpg(char *data, int width, int height);
esp_err_t my_lcd_draw_bitmap(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t *data);
esp_err_t lcd_flush(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t *data);
#endif
