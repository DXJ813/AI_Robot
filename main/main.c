#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "display.h"
#include "camera.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "img_converters.h"
#include "board_config.h"

static const char *TAG = "main";

/* JPEG 解码后 320x240，取中心 240x240 显示 */
#define DECODE_W    320
#define DECODE_H    240
#define DECODE_BUF  (DECODE_W * DECODE_H * 2)
#define CROP_X      ((DECODE_W - BOARD_LCD_H_RES) / 2)   /* 40 */
#define CROP_W      BOARD_LCD_H_RES
#define CROP_H      BOARD_LCD_V_RES
#define DRAW_BUF    (CROP_W * CROP_H * 2)

static void display_test_pattern(esp_lcd_panel_handle_t panel)
{
    static const uint16_t colors[] = { 0xF800, 0x07E0, 0x001F };
    uint16_t *buf = heap_caps_malloc(DRAW_BUF, MALLOC_CAP_INTERNAL);
    if (!buf) return;
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < CROP_W * CROP_H; i++) buf[i] = colors[c];
        bsp_display_draw_bitmap(panel, 0, 0, CROP_W, CROP_H, buf);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
    heap_caps_free(buf);
}

static void draw_camera_frame(esp_lcd_panel_handle_t panel, camera_fb_t *fb)
{
    if (!panel || !fb || !fb->buf || fb->len == 0) return;
    if (fb->format != PIXFORMAT_JPEG) {
        ESP_LOGW(TAG, "非 JPEG 格式: %d，跳过", fb->format);
        return;
    }

    /* 解码到 PSRAM，320x240 RGB565 */
    uint8_t *decode_buf = heap_caps_malloc(DECODE_BUF, MALLOC_CAP_SPIRAM);
    if (!decode_buf) {
        decode_buf = heap_caps_malloc(DECODE_BUF, MALLOC_CAP_INTERNAL);
    }
    if (!decode_buf) return;

    if (!jpg2rgb565(fb->buf, fb->len, decode_buf, JPEG_IMAGE_SCALE_0)) {
        heap_caps_free(decode_buf);
        return;
    }

    /* 取中心 240x240 复制到内部 RAM 后绘制 */
    uint8_t *draw_buf = heap_caps_malloc(DRAW_BUF, MALLOC_CAP_INTERNAL);
    if (draw_buf) {
        const uint16_t *src = (const uint16_t *)decode_buf;
        uint16_t *dst = (uint16_t *)draw_buf;
        for (int y = 0; y < CROP_H; y++) {
            for (int x = 0; x < CROP_W; x++) {
                dst[y * CROP_W + x] = src[y * DECODE_W + (CROP_X + x)];
            }
        }
        bsp_display_draw_bitmap(panel, 0, 0, CROP_W, CROP_H, draw_buf);
        heap_caps_free(draw_buf);
    }

    heap_caps_free(decode_buf);
}

void app_main(void)
{
    esp_lcd_panel_handle_t panel = bsp_display_init();
    if (!panel) {
        ESP_LOGE(TAG, "显示屏初始化失败");
        return;
    }

    display_test_pattern(panel);

    bool camera_ok = (bsp_camera_init() == ESP_OK);
    if (!camera_ok) {
        ESP_LOGW(TAG, "摄像头初始化失败，将显示灰屏");
    } else {
        ESP_LOGI(TAG, "开始显示摄像头画面");
    }

    /* 摄像头失败时也继续循环，避免黑屏 */
    while (1) {
        if (camera_ok) {
            camera_fb_t *fb = bsp_camera_capture();
            if (fb && fb->buf && fb->len > 0) {
                draw_camera_frame(panel, fb);
                bsp_camera_fb_return(fb);
            } else {
                if (fb) bsp_camera_fb_return(fb);
            }
        } else {
            /* 摄像头失败时显示灰色占位 */
            uint16_t *buf = heap_caps_malloc(DRAW_BUF, MALLOC_CAP_INTERNAL);
            if (buf) {
                for (int i = 0; i < CROP_W * CROP_H; i++) buf[i] = 0x7BEF;
                bsp_display_draw_bitmap(panel, 0, 0, CROP_W, CROP_H, buf);
                heap_caps_free(buf);
            }
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        vTaskDelay(pdMS_TO_TICKS(33));
    }
}
