#include "CH59x_common.h"
#include "key_scan.h"
#include "hidkbd.h"       // 引入蓝牙发送功能

// 定义消抖阈值，假设外部定时器每 10ms 调用一次该函数，2 次就是 20ms
#define DEBOUNCE_TICKS  2 

// 定义按键结构体
typedef struct {
    uint8_t  port;             // 端口：'A' 或 'B'
    uint32_t pin;              // 引脚号：如 GPIO_Pin_4
    uint8_t  keycode;          // 预留：该按键对应的常规键码
    uint8_t  modifier;         // 预留：该按键对应的修饰键码
    uint8_t  confirmed_level;  // 经过消抖确认的引脚电平 (1 为松开，0 为按下)
    uint8_t  debounce_cnt;     // 消抖计数器
} Key_t;

// 初始化 9 个按键数组 (初始状态假设引脚被上拉拉高，confirmed_level 填 1)
Key_t my_keys[9] = {
    {'A', GPIO_Pin_4,  0x1E, 0x00, 1, 0}, // 1
    {'A', GPIO_Pin_5,  0x1F, 0x00, 1, 0}, // 2
    {'A', GPIO_Pin_10, 0x20, 0x00, 1, 0}, // 3
    {'A', GPIO_Pin_11, 0x21, 0x00, 1, 0}, // 4
    {'A', GPIO_Pin_12, 0x2A, 0x00, 1, 0}, // Backspace
    {'A', GPIO_Pin_13, 0x2C, 0x00, 1, 0}, // Space

    {'B', GPIO_Pin_4,  0x08, 0x00, 1, 0}, // E
    {'B', GPIO_Pin_7,  0x14, 0x00, 1, 0}, // Q
    {'B', GPIO_Pin_12, 0x1A, 0x00, 1, 0}  // W
};

/*IO口输入配置*/
void GPIO_KeyInput(void)
{
    GPIOA_ModeCfg(GPIO_Pin_4,GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_5,GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_10,GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_11,GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_12,GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_13,GPIO_ModeIN_PU);

    GPIOB_ModeCfg(GPIO_Pin_4,GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_7,GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_12,GPIO_ModeIN_PU);

}

// 读取引脚当前瞬间的物理电平 (不带消抖)
uint8_t Key_Read_Raw_Level(uint8_t port, uint32_t pin)
{
    if (port == 'A')
    {
        return (GPIOA_ReadPortPin(pin) ? 1 : 0);
    } 
    else if (port == 'B') 
    {
        return (GPIOB_ReadPortPin(pin) ? 1 : 0);
    }
    return 1; // 默认返回高电平 (未按下)
}


// 定时扫描按键并消抖
void Key_Scan_Task(void)
{
    for (int i = 0; i < 9; i++) 
    {
        uint8_t current_raw_level = Key_Read_Raw_Level(my_keys[i].port, my_keys[i].pin);

        if (current_raw_level != my_keys[i].confirmed_level) 
        {
            my_keys[i].debounce_cnt++;
            if (my_keys[i].debounce_cnt >= DEBOUNCE_TICKS) 
            {
                my_keys[i].confirmed_level = current_raw_level;
                my_keys[i].debounce_cnt = 0; 

                // --- 跨文件调用蓝牙发送逻辑 ---
                if (my_keys[i].confirmed_level == 0) // 按下
                {
                    // 发送配置好的按键
                    hidEmuSendKbdReport(my_keys[i].modifier, my_keys[i].keycode);
                } 
                else // 松开
                {
                    // 发送全 0，告诉主机按键已松开，防止粘键
                    hidEmuSendKbdReport(0x00, 0x00); 
                }
            }
        } 
        else 
        {
            my_keys[i].debounce_cnt = 0;
        }
    }
}