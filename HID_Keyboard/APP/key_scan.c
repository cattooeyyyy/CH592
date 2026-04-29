#include "CH59x_common.h"
#include "key_scan.h"
#include "hidkbd.h"       // 引入蓝牙发送功能
#include "app_led.h"

// --- 硬件引脚宏定义，方便后续修改布线 ---
// 行线定义 (输出)
#define ROW0_PIN  GPIO_Pin_4  // PA4
#define ROW1_PIN  GPIO_Pin_5  // PA5
#define ROW2_PIN  GPIO_Pin_10 // PA10
#define ROW3_PIN  GPIO_Pin_11 // PA11
#define ROW_PINS_A (ROW0_PIN | ROW1_PIN | ROW2_PIN | ROW3_PIN)

// 列线定义 (输入)
#define COL0_PIN  GPIO_Pin_12 // PA12
#define COL1_PIN  GPIO_Pin_13 // PA13
#define COL_PINS_A (COL0_PIN | COL1_PIN)

#define COL2_PIN  GPIO_Pin_4  // PB4
#define COL3_PIN  GPIO_Pin_7  // PB7
#define COL_PINS_B (COL2_PIN | COL3_PIN)

// LED 指示灯定义 (输出)
#define LED_PIN   GPIO_Pin_12 // PB12


// 定义消抖阈值，假设外部定时器每 10ms 调用一次该函数，2 次就是 20ms
#define DEBOUNCE_TICKS  2 
#define CTRL_KEY_LONG_PRESS_TICKS 125

// 定义物理引脚结构体
typedef struct {
    uint8_t  port;
    uint32_t pin;
} IOPin_t;

// 4根行线 (对应之前的配置)
const IOPin_t ROW_PINS[4] = {
    {'A', GPIO_Pin_4},
    {'A', GPIO_Pin_5},
    {'A', GPIO_Pin_10},
    {'A', GPIO_Pin_11}
};

// 4根列线 (对应之前的配置)
const IOPin_t COL_PINS[4] = {
    {'A', GPIO_Pin_12},
    {'A', GPIO_Pin_13},
    {'B', GPIO_Pin_4},
    {'B', GPIO_Pin_7}
};

// 定义按键结构体
typedef struct {
    uint8_t  keycode;         //键码       
    uint8_t  modifier;        //修饰 
    uint8_t  confirmed_level;  
    uint8_t  debounce_cnt;
} Key_t;

// 初始化按键数组 (初始状态假设引脚被上拉拉高，confirmed_level 填 1)
Key_t my_keys[16] = {
    /* 行0 */
    {0x1E, 0x00, 1, 0}, // 1
    {0x1F, 0x00, 1, 0}, // 2
    {0x20, 0x00, 1, 0}, // 3
    {0x21, 0x00, 1, 0}, // 4
    /* 行1 */
    {0x14, 0x00, 1, 0}, // Q
    {0x1A, 0x00, 1, 0}, // W
    {0x08, 0x00, 1, 0}, // E
    {0x2c, 0x00, 1, 0}, // Space
    /* 行2 */
    {0x50, 0x00, 1, 0}, // Left
    {0x4B, 0x00, 1, 0}, // Page Up
    {0x4E, 0x00, 1, 0}, // Page Down
    {0x4F, 0x00, 1, 0}, // Right
    /* 行3 */
    {0x2A, 0x00, 1, 0}, // Backspace
    {0x2C, 0x04, 1, 0}, // Alt+Space (0x04代表 Left Alt, 0x2C代表 Space)
    {0x1D, 0x01, 1, 0}, // Ctrl+Z (0x01代表 Left Ctrl, 0x1D代表 Z)
    {0x00, 0x00, 1, 0}  // 保留，蓝牙配对控制
};

/*IO口输入配置*/
void GPIO_KeyInput(void)
{
// 1. 配置行线 (Row) 为推挽输出
    GPIOA_ModeCfg(ROW_PINS_A, GPIO_ModeOut_PP_5mA);
    // 初始化时将所有行线拉高，处于非扫描活动状态
    GPIOA_SetBits(ROW_PINS_A); 

    // 2. 配置列线 (Col) 为上拉输入
    // A 端口的列线
    GPIOA_ModeCfg(COL_PINS_A, GPIO_ModeIN_PU);
    // B 端口的列线
    GPIOB_ModeCfg(COL_PINS_B, GPIO_ModeIN_PU);

    // 3. 配置 LED 指示灯为推挽输出
    GPIOB_ModeCfg(LED_PIN, GPIO_ModeOut_PP_5mA);
    // 默认输出低电平（如果你的硬件是高电平点亮，这里即为默认熄灭；若是低电平点亮，请改为 GPIOB_SetBits）
    GPIOB_ResetBits(LED_PIN);
}

// 设置指定行的电平 (0为拉低，1为拉高)
static void Set_Row_Level(uint8_t row_index, uint8_t level)
{
    if (ROW_PINS[row_index].port == 'A') {
        if (level) GPIOA_SetBits(ROW_PINS[row_index].pin);
        else       GPIOA_ResetBits(ROW_PINS[row_index].pin);
    } else {
        if (level) GPIOB_SetBits(ROW_PINS[row_index].pin);
        else       GPIOB_ResetBits(ROW_PINS[row_index].pin);
    }
}

// 读取指定列的电平
static uint8_t Read_Col_Level(uint8_t col_index)
{
    if (COL_PINS[col_index].port == 'A') {
        return (GPIOA_ReadPortPin(COL_PINS[col_index].pin) ? 1 : 0);
    } else {
        return (GPIOB_ReadPortPin(COL_PINS[col_index].pin) ? 1 : 0);
    }
}


// 定时扫描按键并消抖
void Key_Scan_Task(void)
{
// 专门用于控制键的长按状态计数器，定义为静态变量以在多次调用间保持数值
    static uint16_t ctrl_hold_cnt = 0; 

    for (uint8_t row = 0; row < 4; row++) 
    {
        // 1. 将当前扫描的行拉低
        Set_Row_Level(row, 0);
        
        // 插入极短暂的延时，确保引脚电平状态稳定后再读取
        __nop(); __nop(); __nop(); __nop();

        // 2. 依次读取 4 根列线的状态
        for (uint8_t col = 0; col < 4; col++) 
        {
            uint8_t current_raw_level = Read_Col_Level(col);
            uint8_t idx = (row * 4) + col;

            // -----------------------------------------------------------
            // 拦截特殊控制键 (假设是矩阵最后一个按键，即 idx == 15)
            // -----------------------------------------------------------
            if (idx == 15) 
            {
                if (current_raw_level == 0) // 检测到物理按下
                {
                    if (ctrl_hold_cnt < CTRL_KEY_LONG_PRESS_TICKS) 
                    {
                        ctrl_hold_cnt++;
                        if (ctrl_hold_cnt == CTRL_KEY_LONG_PRESS_TICKS) 
                        {
                            // 计数器达到长按阈值，触发断开连接
                            HidEmu_Disconnect();
                        }
                    }
                    // 只要持续按下，计数器保持在阈值，不会溢出
                } 
                else // 物理松开或短按
                {
                    ctrl_hold_cnt = 0; // 松开即刻清零
                }
                
                // 处理完控制键逻辑后，直接跳过当前循环，不将其视作常规键码发送
                continue; 
            }

            // -----------------------------------------------------------
            // 普通按键 (idx 0 ~ 14) 的常规消抖与键码发送逻辑
            // -----------------------------------------------------------
            if (current_raw_level != my_keys[idx].confirmed_level) 
            {
                my_keys[idx].debounce_cnt++;
                if (my_keys[idx].debounce_cnt >= DEBOUNCE_TICKS) 
                {
                    my_keys[idx].confirmed_level = current_raw_level;
                    my_keys[idx].debounce_cnt = 0; 

                    if (my_keys[idx].confirmed_level == 0) // 确认按下
                    {
                        hidEmuSendKbdReport(my_keys[idx].modifier, my_keys[idx].keycode);
                    } 
                    else // 确认松开
                    {
                        hidEmuSendKbdReport(0x00, 0x00); 
                    }
                }
            } 
            else 
            {
                // 若电平未发生变化，消抖计步器清零
                my_keys[idx].debounce_cnt = 0;
            }
        }
        
        // 3. 扫描完该行后重新拉高，避免干扰下一行扫描
        Set_Row_Level(row, 1);
    }
    }