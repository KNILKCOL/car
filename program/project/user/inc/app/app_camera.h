#ifndef _app_camera_h_
#define _app_camera_h_

#include <stdint.h>

#define CAM_IMG_H   120
#define CAM_IMG_W   188

// 声明外部可以调用的函数
void app_camera_init(void);
float app_camera_get_error(void);

#endif