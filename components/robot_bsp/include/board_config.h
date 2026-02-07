/*
 * * 硬件引脚配置文件
*/
#pragma once

#include "driver/gpio.h"
#include "driver/i2c.h"

/* * ============================================================
 * 系统与交互 (System & Interaction)
 * ------------------------------------------------------------
 */
#define BOARD_BUTTON_BOOT       GPIO_NUM_0   // 系统自带 Boot 键
#define BOARD_BUTTON_ADC_IO     GPIO_NUM_1   // 矩阵按键 ADC 输入口 (多按键共用一个IO)
#define BOARD_PA_ENABLE         GPIO_NUM_NC  // 功放使能脚 (NC表示常开或由Codec控制)


/* * ============================================================
 * I2C 总线 (I2C Bus)
 * ------------------------------------------------------------
 * 挂载多个设备：
 * - 摄像头 (OV2640 SCCB)
 * - 陀螺仪/加速度计
 * - 音频 Codec 控制 (ES8311 / ES7210)
 * ============================================================
 */
#define BOARD_I2C_SDA           GPIO_NUM_4
#define BOARD_I2C_SCL           GPIO_NUM_5
#define BOARD_I2C_PORT          I2C_NUM_0
#define BOARD_I2C_FREQ_HZ       100000       // 100kHz 标准速率


/* * ============================================================
 * 屏幕 (ST7789)
 * ------------------------------------------------------------
 * 接口类型: SPI
 * 分辨率: 240x240
 * ============================================================
 */
#define BOARD_LCD_MOSI          GPIO_NUM_47
#define BOARD_LCD_CLK           GPIO_NUM_21
#define BOARD_LCD_CS            GPIO_NUM_44
#define BOARD_LCD_DC            GPIO_NUM_43
#define BOARD_LCD_RST           GPIO_NUM_NC  // 软件复位
#define BOARD_LCD_BK_LIGHT      GPIO_NUM_46  // 背光控制

#define BOARD_LCD_H_RES         240
#define BOARD_LCD_V_RES         240


/* * ============================================================
 * 摄像头 (OV2640)
 * ------------------------------------------------------------
 * 接口类型: DVP (8-bit Parallel)
 * 控制接口: 使用板载 I2C 总线 (SCCB, 地址 0x30)
 * ============================================================
 */
#define BOARD_CAM_XCLK          GPIO_NUM_15
#define BOARD_CAM_PCLK          GPIO_NUM_13
#define BOARD_CAM_VSYNC         GPIO_NUM_6
#define BOARD_CAM_HREF          GPIO_NUM_7

#define BOARD_CAM_D0            GPIO_NUM_11
#define BOARD_CAM_D1            GPIO_NUM_9
#define BOARD_CAM_D2            GPIO_NUM_8
#define BOARD_CAM_D3            GPIO_NUM_10
#define BOARD_CAM_D4            GPIO_NUM_12
#define BOARD_CAM_D5            GPIO_NUM_18
#define BOARD_CAM_D6            GPIO_NUM_17
#define BOARD_CAM_D7            GPIO_NUM_16

#define BOARD_CAM_SIOD          BOARD_I2C_SDA
#define BOARD_CAM_SIOC          BOARD_I2C_SCL


/* * ============================================================
 * 音频系统 (ES8311 + ES7210)
 * ------------------------------------------------------------
 * 接口类型: I2S (全双工)
 * 芯片方案: 
 * - ES8311 (DAC/扬声器驱动)
 * - ES7210 (ADC/麦克风录音)
 * ============================================================
 */
#define BOARD_I2S_MCLK          GPIO_NUM_45  // Master Clock (为 Codec 提供基准时钟)
#define BOARD_I2S_BCK           GPIO_NUM_39  // Bit Clock (SCLK)
#define BOARD_I2S_WS            GPIO_NUM_41  // Word Select (LRCK/LCLK)
#define BOARD_I2S_DO            GPIO_NUM_42  // Data Out (ESP32 -> ES8311 播放)
#define BOARD_I2S_DI            GPIO_NUM_40  // Data In  (ES7210 -> ESP32 录音)

// 音频芯片的 I2C 地址 (备查，后续写 audio.c 时会用到)
// ES8311_ADDR: 0x18
// ES7210_ADDR: 0x40