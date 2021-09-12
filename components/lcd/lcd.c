/* LCD tjpgd example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "decode_image.h"
#include <pthread.h>
#include "lcd.h"
#include "gui.h"

/**
 * If not using the default settings, the SPI peripheral on LCD and the GPIO numbers can be
 * changed below.
*/
#define LCD_HOST    SPI2_HOST   /*!< spi peripheral for LCD */

#define PIN_NUM_MOSI 13     /*!< gpio number for LCD MOSI */
#define PIN_NUM_CLK  12     /*!< gpio number for LCD clock */
#define PIN_NUM_CS   14      /*!< gpio number for LCD CS */
#define PIN_NUM_DC   27     /*!< gpio number for LCD DC */
#define PIN_NUM_RST  15     /*!< gpio number for LCD RST */
#define PIN_NUM_BCKL 2      /*!< gpio number for LCD Back Light */

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES (240)
#define EXAMPLE_LCD_V_RES (320)

// To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many.
// More means more memory use, but less overhead for setting up / finishing transfers. Make sure 240
// is dividable by this.
#define PARALLEL_LINES 30

// Simple routine to generate some patterns and send them to the LCD. Because the
// SPI driver handles transactions in the background, we can calculate the next line
// while the previous one is being sent.
static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_panel_io_handle_t io_handle = NULL;
static uint16_t *s_lines = NULL;
static uint16_t **pixels = NULL;

esp_err_t lcd_flush(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t *data)
{
    if (panel_handle == NULL || data == NULL) {
		return ESP_FAIL;
	}

    if (esp_lcd_panel_draw_bitmap(panel_handle, xs, ys, xe, ye, data)) {
        printf("lcd draw bitmap failed\n");
        return ESP_FAIL;
    }

    return esp_lcd_panel_write_reg(panel_handle, 0x2c, NULL, 0);
}

static esp_err_t lcd_draw_bitmap(uint16_t x_s, uint16_t y_s, uint16_t x_e, uint16_t y_e)
{
	if (panel_handle == NULL || s_lines == NULL) {
		return ESP_FAIL;
	}

	return esp_lcd_panel_draw_bitmap(panel_handle, x_s, y_s, x_e, y_e, s_lines);
}

esp_err_t get_jpg_data(uint16_t *dest, int line, int linect)
{
    if (dest == NULL || pixels == NULL) {
        printf("dest or source null\n");
        return ESP_FAIL;
    }

    for (int y=line; y<line+linect; y++) {
        for (int x=0; x<320; x++) {
            *dest++ = pixels[y][x];
        }
    }

    return ESP_OK;
}

esp_err_t lcd_display_jpg(char *data, int width, int height)
{
    esp_err_t ret;

	if (data == NULL) {
		printf("input data null\n");
		return ESP_FAIL;
	}

	ret = decode_jpg(&pixels, data, width, height);
    if (ret) {
		printf("jpg decode failed\n");
        return ESP_FAIL;
    }

    for (int y = 0; y < EXAMPLE_LCD_V_RES; y += PARALLEL_LINES) {
        if (get_jpg_data(s_lines, y, PARALLEL_LINES)) {
			return ESP_FAIL;
		}
        lcd_draw_bitmap( 0, y, 0 + EXAMPLE_LCD_H_RES, y + PARALLEL_LINES);
    }

    return ESP_OK;
}

static void *gui_loop(void *arg)
{
    guiobj_t *gui = NULL;
	char teststr[6][32];

    gui = gui_create(EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);
    if (gui == NULL) {
        printf("gui create failed\n");
        return NULL;
    }

    gui->gui_start(lcd_flush, GUI_WHITE, GUI_BLACK);

	while (1) {
        /* clear canvas */
        gui->ops.clear();

        /* draw line */
        gui->ops.draw_line(rand() % 240, rand() % 320, rand() % 240, rand() % 320, 0, rand() % GUI_COLOR_MAX);

        /* draw rectangle */
        gui->ops.draw_rectangle(rand() % 240, rand() % 320, rand() % 240, rand() % 320, 0, rand() % GUI_COLOR_MAX);

        /* fill rectangle */
        // gui->ops.fill(rand() % 240, rand() % 320, rand() % 240, rand() % 320, rand() % GUI_COLOR_MAX);

        /* draw circle */
        gui->ops.draw_circle(rand() % 240, rand() % 320, rand() % 60, rand() % GUI_COLOR_MAX, 1);

        /* show string */
        // sprintf((char *)teststr[0], "-%3d mV", gui ? 1 : 0);
		// sprintf((char *)teststr[1], "CH1: %3d mV 123455678", rand() % 54);
		// sprintf((char *)teststr[2], "CH2: ");
		// sprintf((char *)teststr[3], " mV 48+-%3d", rand() % 99);
		// sprintf((char *)teststr[4], "CH4: %04d mA", rand() %1000);
		// sprintf((char *)teststr[5], "CH5: %04d mA", rand() % 10000);
        // gui->ops.show_string(100, 70, 16, teststr[0], strlen(teststr[0]), GUI_RED);
        // gui->ops.show_string(100, 86, 16, teststr[1], strlen(teststr[1]), GUI_YELLOW);
        // gui->ops.show_string(20, 70, 32, teststr[2], strlen(teststr[2]), GUI_AUTO);
        // gui->ops.show_string(10, 102, 48, teststr[3], strlen(teststr[3]), GUI_GREEN);
        // gui->ops.show_string(5, 200, 32, teststr[4], strlen(teststr[4]), GUI_MAGENTA);
        // gui->ops.show_string(10, 307, 32, teststr[5], strlen(teststr[5]), GUI_GRAYBLUE);

        /* to display */
        gui->ops.flush();

		vTaskDelay(300 / portTICK_PERIOD_MS);
	}

    printf("exit gui loop\n");
    return NULL;
}

guiobj_t *gui = NULL;

esp_err_t my_lcd_draw_bitmap(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t *data)
{
    if (gui == NULL) {
        printf("canvas has not init\n");
        return -1;
    }

    return gui->ops.draw_bitmap(xs, ys, xe, ye, data);
}

int lcd_init(void)
{
    pthread_t tid;
    pthread_attr_t attr = {.stacksize = 1024 * 8};
    int ret;

    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_BCKL
    };
    // Initialize the GPIO of backlight
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    spi_bus_config_t buscfg = {
        .miso_io_num = -1,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 320 * EXAMPLE_LCD_H_RES * 2 + 8
    };

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,
        .cs_gpio_num = PIN_NUM_CS,
        .pclk_hz = 26 * 1000 * 1000,
        .spi_mode = 0,
        .trans_queue_depth = 7,
    };
    // Initialize the SPI bus
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,
        .color_space = ESP_LCD_COLOR_SPACE_BGR,
        .bits_per_pixel = 16,
    };
    // Initialize the LCD configuration
    ESP_ERROR_CHECK(esp_lcd_new_panel_iti9341(io_handle, &panel_config, &panel_handle));

    // Turn off backlight to avoid unpredictable display on the LCD screen while initializing
    // the LCD panel driver. (Different LCD screens may need different levels)
    ESP_ERROR_CHECK(gpio_set_level(PIN_NUM_BCKL, 1));

    // Reset the display
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

    // Initialize LCD panel
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    // Swap x and y axis (Different LCD screens may need different options)
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));

    // Turn on backlight (Different LCD screens may need different levels)
    ESP_ERROR_CHECK(gpio_set_level(PIN_NUM_BCKL, 1));

	esp_lcd_panel_mirror(panel_handle, 0, 1);

    // gui = gui_create(EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);
    // if (gui == NULL) {
    //     printf("gui create failed\n");
    //     return -1;
    // }

    // gui->gui_start(lcd_flush, GUI_WHITE, GUI_BLACK);

    // ret = pthread_create(&tid, &attr, gui_loop, NULL);
    // if (ret) {
    //     printf("led thread create failed\n");
    //     return -1;
    // }
    // pthread_join(tid, NULL);

    printf("lcd task start\n");
    return 0;
}