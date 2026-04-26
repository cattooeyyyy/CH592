#include "CH59x_common.h"

void GPIO_KeyInput(void);
extern void hidEmuSendKbdReport(uint8_t modifier, uint8_t keycode);
void Key_Scan_Task(void);