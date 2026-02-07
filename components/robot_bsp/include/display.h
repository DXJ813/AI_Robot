/* components/robot_bsp/include/display.h */
#pragma once

#include "esp_err.h"
#include <stdbool.h>
#include "esp_lcd_panel_ops.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化显示屏 (SPI ST7789)
 * 初始化 SPI 总线，挂载屏幕，点亮背光。
 * @return esp_lcd_panel_handle_t 屏幕句柄 (给绘图函数用)
 */
esp_lcd_panel_handle_t bsp_display_init(void);

/**
 * @brief 在屏幕上绘制位图 (RGB565)
 * @param panel 屏幕句柄 (由 bsp_display_init 返回)
 * @param x_start 起始 X 坐标 (含)
 * @param y_start 起始 Y 坐标 (含)
 * @param x_end 结束 X 坐标 (不含)
 * @param y_end 结束 Y 坐标 (不含)
 * @param color_data RGB565 像素数据指针
 */
esp_err_t bsp_display_draw_bitmap(esp_lcd_panel_handle_t panel,
                                  int x_start, int y_start, int x_end, int y_end,
                                  const void *color_data);

/**
 * @brief 开启或关闭背光
 * @param en true: 开启, false: 关闭
 */
void bsp_display_backlight(bool en);

// 为了方便 C++ 调用 C，必须加这个结束符
#ifdef __cplusplus
}
#endif