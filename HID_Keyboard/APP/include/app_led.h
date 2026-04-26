#ifndef APP_LED_H
#define APP_LED_H

#include "CH59x_common.h"
#include "CONFIG.h"

// LED 初始化函数，需在系统启动时调用
void AppLed_Init(void);

// LED 状态控制函数
// state: 1 表示已连接，0 表示未连接(广播中)
// bondCount: 当前设备记录的配对主机数量，用于区分快闪和慢闪
void AppLed_Set_State(uint8_t state, uint8_t bondCount);

#endif