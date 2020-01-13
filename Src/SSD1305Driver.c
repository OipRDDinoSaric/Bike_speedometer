#include "SSD1305Driver.h"

void SSD1305_DisplaySpeed (int speed)
{
	char buffer[10];
	ssd1306_Fill (Black);
	ssd1306_SetCursor (2, 0);
	ssd1306_WriteString ("Velocity: ", Font_11x18, White);
	ssd1306_SetCursor (2, 18);
	itoa (speed, buffer, 10);
	strlcat (buffer, " kmph", 10);
	ssd1306_WriteString (buffer, Font_11x18, White);
	//ssd1306_UpdateScreen ();
}

void SSD1305_DisplayAlarmStatus (void)
{
	if (AlarmFlag)
	{
		ssd1306_SetCursor (2, 2 * 18);
		ssd1306_WriteString ("Alarm ON", Font_11x18, White);
	}
	else
	{
		ssd1306_SetCursor (2, 2 * 18);
		ssd1306_WriteString ("Alarm OFF", Font_11x18, White);
	}
	//ssd1306_UpdateScreen ();
}

void SSD1305_DisplayAlarmON (uint32_t timeout)
{
	ssd1306_Fill (Black);
	ssd1306_SetCursor (2, 2 * 18);
	ssd1306_WriteString ("Alarm ON", Font_11x18, White);
	SSD1305_UpdateScreen ();
	HAL_Delay (timeout);
	ssd1306_Fill (Black);
	SSD1305_UpdateScreen ();
}

void SSD1305_UpdateScreen ()
{
	ssd1306_UpdateScreen ();
}

void SSD1305_FillBlack()
{
	ssd1306_Fill (Black);
}

void SSD1305_Init (void)
{
	ssd1306_Init ();
}
