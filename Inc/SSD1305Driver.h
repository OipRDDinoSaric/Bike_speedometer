#include <stdlib.h>
#include "ssd1306.h"
#include <string.h>

extern uint8_t AlarmFlag;

void SSD1305_DisplaySpeed (int speed);
void SSD1305_DisplayAlarmStatus (void);
void SSD1305_DisplayAlarmON (uint32_t timeout);
void SSD1305_UpdateScreen ();
void SSD1305_FillBlack();
void SSD1305_Init (void);
