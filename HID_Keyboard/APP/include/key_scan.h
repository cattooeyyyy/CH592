#include "CH59x_common.h"

void GPIO_KeyInput(void);
uint8_t Key_Read_Raw_Level(uint8_t port, uint32_t pin);
extern void hidEmuSendKbdReport(uint8_t modifier, uint8_t keycode);
void Key_Scan_Task(void);