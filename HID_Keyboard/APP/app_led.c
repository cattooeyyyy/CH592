#include "app_led.h"
#include "CONFIG.h"
#include "CH59x_common.h"
// 定义 LED 闪烁专属事件标志位。
// TMOS 的事件标志位是以 bit 为单位的，0x0001 即 0000 0000 0000 0001
#define LED_BLINK_EVT        0x0001  

// 定义 LED 物理引脚
#define LED_PIN              GPIO_Pin_12 

// 定义 TMOS 任务 ID。初始化为无效值，稍后由 TMOS 系统自动分配
static uint8_t AppLed_TaskId = INVALID_TASK_ID; 

// 记录当前的闪烁间隔（单位：tick，1 tick ≈ 0.625 毫秒）
static uint16_t current_led_interval = 0;


// 这是 TMOS 回调的核心函数。当 LED_BLINK_EVT 被触发时，系统会进入这里
uint16_t AppLed_ProcessEvent(uint8_t task_id, uint16_t events)
{
    // 检查是否是我们的 LED 闪烁事件
    if(events & LED_BLINK_EVT)
    {
        // 1. 执行动作：翻转 PB12 的引脚电平，实现亮灭交替
        GPIOB_InverseBits(LED_PIN);
        
        // 2. 状态维持：如果 current_led_interval 大于 0，说明需要持续闪烁
        if(current_led_interval > 0) 
        {
            // 设定一个定时器，在 current_led_interval 个 tick 之后，再次触发 LED_BLINK_EVT
            tmos_start_task(AppLed_TaskId, LED_BLINK_EVT, current_led_interval);
        }
        
        // 3. 清除事件标志：将已经处理完的 LED_BLINK_EVT 从 events 中剔除，返回给 TMOS
        return (events ^ LED_BLINK_EVT);
    }
    
    // 如果收到未知事件，不作处理，直接返回 0
    return 0; 
}

void AppLed_Init(void)
{
    // 1. 向 TMOS 注册我们的处理函数。TMOS 会返回一个独一无二的 TaskId
    // 以后所有关于这个 LED 的定时和事件发送，都要拿着这个 TaskId 去申请
    AppLed_TaskId = TMOS_ProcessEventRegister(AppLed_ProcessEvent);
    
    // 2. 硬件初始化：配置 PB12 为推挽输出，并默认输出低电平（灭）
    GPIOB_ModeCfg(LED_PIN, GPIO_ModeOut_PP_5mA);
    GPIOB_ResetBits(LED_PIN); 
}


void AppLed_Set_State(uint8_t state, uint8_t bondCount)
{
    if (state == 1) // 状态 3：已稳定连接
    {
        // 停止之前的闪烁定时器，防止干扰
        tmos_stop_task(AppLed_TaskId, LED_BLINK_EVT);
        // 将间隔清零，切断闪烁的“接力赛”
        current_led_interval = 0;
        // 将 PB12 置为低电平（长灭）。如果你的硬件是低电平点亮，这里应改为 GPIOB_SetBits
        GPIOB_ResetBits(LED_PIN); 
    } 
    else if (state == 0) // 状态 1 和 2：广播搜寻中 / 回连中
    {
        // 切换频率前，先停止正在运行的定时任务，避免新老任务重叠触发导致闪烁紊乱
        tmos_stop_task(AppLed_TaskId, LED_BLINK_EVT);
        
        // 判断历史绑定数量：
        // 如果 bondCount > 0 (已有配对)，间隔设为 1600 ticks (约 1000ms)，慢闪
        // 如果 bondCount == 0 (全新配对)，间隔设为 320 ticks (约 200ms)，快闪
        current_led_interval = (bondCount > 0) ? 1600 : 320; 
        
        // 立即启动第一次闪烁任务
        tmos_start_task(AppLed_TaskId, LED_BLINK_EVT, current_led_interval);
    }
}