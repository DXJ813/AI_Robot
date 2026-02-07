/* components/robot_bsp/include/camera.h */
#pragma once

#include "esp_err.h"
#include "esp_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化摄像头 (OV2640 DVP)
 *
 * 根据 board_config.h 中的引脚配置初始化 OV2640 摄像头。
 * 默认输出 240x240 RGB565，与屏幕分辨率一致，便于直接显示。
 *
 * @return ESP_OK 成功，其他为失败
 */
esp_err_t bsp_camera_init(void);

/**
 * @brief 反初始化摄像头
 * @return ESP_OK 成功
 */
esp_err_t bsp_camera_deinit(void);

/**
 * @brief 捕获一帧图像
 *
 * @return 帧缓冲指针，失败返回 NULL。使用完毕后需调用 bsp_camera_fb_return() 归还
 */
camera_fb_t *bsp_camera_capture(void);

/**
 * @brief 归还帧缓冲
 * @param fb 由 bsp_camera_capture() 获取的帧缓冲指针
 */
void bsp_camera_fb_return(camera_fb_t *fb);

/**
 * @brief 获取传感器句柄，用于高级控制（亮度、对比度、镜像等）
 * @return sensor_t* 传感器指针，未初始化时返回 NULL
 */
sensor_t *bsp_camera_sensor_get(void);

#ifdef __cplusplus
}
#endif
