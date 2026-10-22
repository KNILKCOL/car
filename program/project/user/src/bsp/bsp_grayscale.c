#include "bsp_grayscale.h"

void bsp_grayscale_init(void) {
    gpio_init(GRAY_1_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(GRAY_2_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(GRAY_3_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(GRAY_4_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(GRAY_5_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
}

float bsp_grayscale_get_error(void) {
    // 假设压黑线为0，白底为1，根据实际探头调整
    int v1 = gpio_get_level(GRAY_1_PIN);
    int v2 = gpio_get_level(GRAY_2_PIN);
    int v3 = gpio_get_level(GRAY_3_PIN);
    int v4 = gpio_get_level(GRAY_4_PIN);
    int v5 = gpio_get_level(GRAY_5_PIN);
    
    // 权重法：左为负，右为正
    float error = 0;
    int count = 0;
    if(v1==0) { error += -2.0; count++; }
    if(v2==0) { error += -1.0; count++; }
    if(v3==0) { error +=  0.0; count++; }
    if(v4==0) { error +=  1.0; count++; }
    if(v5==0) { error +=  2.0; count++; }
    
    if(count == 0) return 99.0; // 丢线
    return error / count;
}