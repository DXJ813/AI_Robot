/* components/robot_bsp/src/camera.c */
#include <stdio.h>
#include "esp_log.h"
#include "esp_camera.h"
#include "board_config.h"
#include "camera.h"

static const char *TAG = "BSP_CAMERA";

#if ESP_CAMERA_SUPPORTED

static camera_config_t s_camera_config = {
    .pin_pwdn = -1,
    .pin_reset = -1,
    .pin_xclk = BOARD_CAM_XCLK,
    .pin_sccb_sda = BOARD_CAM_SIOD,
    .pin_sccb_scl = BOARD_CAM_SIOC,
    .pin_d7 = BOARD_CAM_D7,
    .pin_d6 = BOARD_CAM_D6,
    .pin_d5 = BOARD_CAM_D5,
    .pin_d4 = BOARD_CAM_D4,
    .pin_d3 = BOARD_CAM_D3,
    .pin_d2 = BOARD_CAM_D2,
    .pin_d1 = BOARD_CAM_D1,
    .pin_d0 = BOARD_CAM_D0,
    .pin_vsync = BOARD_CAM_VSYNC,
    .pin_href = BOARD_CAM_HREF,
    .pin_pclk = BOARD_CAM_PCLK,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 10,
    .fb_count = 1,
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_LATEST,
};

#endif

esp_err_t bsp_camera_init(void)
{
#if ESP_CAMERA_SUPPORTED
    ESP_LOGI(TAG, "开始初始化摄像头 (OV2640)...");

    esp_err_t err = esp_camera_init(&s_camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "摄像头初始化失败: 0x%x", err);
        return err;
    }

    ESP_LOGI(TAG, "摄像头初始化完成 (QVGA JPEG)");
    return ESP_OK;
#else
    ESP_LOGE(TAG, "当前芯片不支持摄像头");
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

esp_err_t bsp_camera_deinit(void)
{
#if ESP_CAMERA_SUPPORTED
    return esp_camera_deinit();
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

camera_fb_t *bsp_camera_capture(void)
{
#if ESP_CAMERA_SUPPORTED
    return esp_camera_fb_get();
#else
    (void)0;
    return NULL;
#endif
}

void bsp_camera_fb_return(camera_fb_t *fb)
{
#if ESP_CAMERA_SUPPORTED
    if (fb) {
        esp_camera_fb_return(fb);
    }
#else
    (void)fb;
#endif
}

sensor_t *bsp_camera_sensor_get(void)
{
#if ESP_CAMERA_SUPPORTED
    return esp_camera_sensor_get();
#else
    return NULL;
#endif
}
