/* components/robot_bsp/include/display.h */
#pragma once

#include "esp_err.h"
#include <stdbool.h>
//#include "esp_lcd_panel_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化显示屏 (SPI ST7789)
 * * 这一步会初始化 SPI 总线，挂载屏幕，并清屏。
 * * @return esp_lcd_panel_handle_t 屏幕句柄 (给 LVGL 或绘图函数用)
 */
//esp_lcd_panel_handle_t bsp_display_init(void);

/**
 * @brief 开启或关闭背光
 * * @param en true: 开启, false: 关闭
 */
void bsp_display_backlight(bool en);

// 为了方便 C++ 调用 C，必须加这个结束符
#ifdef __cplusplus
}
#endif