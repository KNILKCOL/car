#include "app_camera.h"

// 申请大数组留给图像处理，不调用不占空间 (这 22KB 会自动填 0)
uint8_t camera_image_buffer[CAM_IMG_H][CAM_IMG_W]; 

// 初始化空壳
void app_camera_init(void) {
    // 等以后有摄像头了，在这里写 DVP 或 UART 初始化
}

// 处理空壳
float app_camera_get_error(void) {
    // 现在没接摄像头，永远返回 0 偏差，不影响灰度寻线
    return 0.0f;
}