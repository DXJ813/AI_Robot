idf_component_register(SRCS "src/display.c" "src/camera.c"
                       INCLUDE_DIRS "include"
                       REQUIRES driver esp_lcd esp32-camera esp_codec_dev)