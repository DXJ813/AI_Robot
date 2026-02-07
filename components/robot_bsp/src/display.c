/* components/robot_bsp/src/display.c */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "esp_err.h"

// 引入我们刚才辛辛苦苦整理的引脚定义
#include "board_config.h"
#include "display.h"

static const char *TAG = "BSP_DISPLAY";

// 这是一个 helper 函数，不公开给外部
static void bsp_display_init_backlight(void)
{
    // 配置背光引脚为输出模式
    gpio_config_t bk_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << BOARD_LCD_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_cfg));
}

void bsp_display_backlight(bool en)
{
    // 设置电平：1 亮，0 灭
    gpio_set_level(BOARD_LCD_BK_LIGHT, en ? 1 : 0);
    ESP_LOGI(TAG, "背光状态: %s", en ? "ON" : "OFF");
}

esp_lcd_panel_handle_t bsp_display_init(void)
{
    ESP_LOGI(TAG, "开始初始化显示屏 (ST7789)...");

    // 1. 初始化背光引脚 (先关掉，防止初始化时闪屏)
    bsp_display_init_backlight();
    bsp_display_backlight(false);

    // 2. 初始化 SPI 总线
    // ESP32-S3 通常使用 SPI2_HOST (也叫 FSPI) 作为主要的显示接口
    spi_bus_config_t buscfg = {
        .sclk_io_num = BOARD_LCD_CLK,
        .mosi_io_num = BOARD_LCD_MOSI,
        .miso_io_num = -1, // 屏幕通常不需要读数据 (MISO)，省一个引脚
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        // 传输最大载荷：全屏像素 * 2字节(RGB565)
        .max_transfer_sz = BOARD_LCD_H_RES * BOARD_LCD_V_RES * sizeof(uint16_t)
    };
    
    // 使用 DMA 通道 (SPI_DMA_CH_AUTO 让系统自动分配)
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // 3. 创建 LCD 面板 IO 句柄 (告诉驱动这是一个 SPI 接口的屏)
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = BOARD_LCD_DC,
        .cs_gpio_num = BOARD_LCD_CS,
        .pclk_hz = 40 * 1000 * 1000, // 40MHz 像素时钟 (ST7789 跑 40M 没问题)
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,               // Mode 0 是最常用的
        .trans_queue_depth = 10,     // 传输队列深度
    };
    // 关联 SPI2_HOST 和 IO配置
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &io_handle));

    // 4. 创建 ST7789 面板句柄
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BOARD_LCD_RST, // 之前在 board_config.h 里定义为 NC (-1)
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,            // RGB565 格式
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    // 5. 屏幕复位与初始化指令
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    // 6. 调整屏幕方向与颜色
    // ST7789 在不同厂家封装下，颜色可能是反的 (Invert)，需要测试
    // 如果你发现黑色变成了白色，把 true 改成 false
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    
    // 设置显示间隙 (Gap)，如果屏幕边缘有花屏或黑边，需要调整这里
    // 240x240 的 ST7789 很多并没有 Gap，先设为 0
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 0, 0));

    // 开启显示
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    // 7. 点亮背光
    ESP_LOGI(TAG, "屏幕初始化完成，开启背光");
    bsp_display_backlight(true);

    return panel_handle;
}

esp_err_t bsp_display_draw_bitmap(esp_lcd_panel_handle_t panel,
                                  int x_start, int y_start, int x_end, int y_end,
                                  const void *color_data)
{
    if (panel == NULL || color_data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    return esp_lcd_panel_draw_bitmap(panel, x_start, y_start, x_end, y_end, color_data);
}