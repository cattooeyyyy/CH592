#include "CH59x_common.h"
#include "key_scan.h"
#include "hidkbd.h"       // 引入蓝牙发送功能

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

// 初始化 9 个按键数组 (初始状态假设引脚被上拉拉高，confirmed_level 填 1)
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
    {0x15, 0x00, 1, 0}, // R
    /* 行2 */
    {0x04, 0x00, 1, 0}, // A
    {0x16, 0x00, 1, 0}, // S
    {0x07, 0x00, 1, 0}, // D
    {0x09, 0x00, 1, 0}, // F
    /* 行3 */
    {0x2A, 0x00, 1, 0}, // Backspace
    {0x2C, 0x00, 1, 0}, // Space
    {0x28, 0x00, 1, 0}, // Enter
    {0x29, 0x00, 1, 0}  // Esc
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
for (uint8_t row = 0; row < 4; row++) 
    {
        // 1. 将当前扫描的行拉低
        Set_Row_Level(row, 0);
        
        // 插入极短暂的延时，确保引脚电平状态稳定后再读取（CH592主频较高时建议添加）
        __nop(); __nop(); __nop(); __nop();

        // 2. 依次读取 4 根列线的状态
        for (uint8_t col = 0; col < 4; col++) 
        {
            uint8_t current_raw_level = Read_Col_Level(col);
            
            // 计算当前按键在 my_keys 数组中的一维索引 (0 ~ 15)
            uint8_t idx = (row * 4) + col;

            // 3. 执行与之前相同的消抖逻辑
            if (current_raw_level != my_keys[idx].confirmed_level) 
            {
                my_keys[idx].debounce_cnt++;
                if (my_keys[idx].debounce_cnt >= DEBOUNCE_TICKS) 
                {
                    my_keys[idx].confirmed_level = current_raw_level;
                    my_keys[idx].debounce_cnt = 0; 

                    // 按键动作确认，调用蓝牙发送逻辑
                    if (my_keys[idx].confirmed_level == 0) // 被按下
                    {
                        hidEmuSendKbdReport(my_keys[idx].modifier, my_keys[idx].keycode);
                    } 
                    else // 被松开
                    {
                        hidEmuSendKbdReport(0x00, 0x00); 
                    }
                }
            } 
            else 
            {
                // 如果电平未发生持续改变，清除消抖计数
                my_keys[idx].debounce_cnt = 0;
            }
        }
        
        // 4. 扫描完该行后，务必将其重新拉高，避免干扰下一行的扫描
        Set_Row_Level(row, 1);
    }}