// init SIM808
// SEND SMS
// GET GPS LOCATION
// receive UART data
// send command
// send UART data
// send at
#include "SIM808Driver.h"

// enter phone number you want to contact
const char* GSM_contact = "0989230034";

SIM808Driver SIM808Handle;
volatile uint8_t SIM808_rx_byte;
volatile uint8_t SIM808_rx_buf[SIM808_RX_SIZE] = { 0 };
volatile uint32_t SIM808_rx_head = 0;
volatile uint8_t SIM808_flag_rx_irq = 0;

void SIM808_Init (void)
{
	SIM808_SendString ("AT\r");
	HAL_Delay (1000);
	if (HAL_UART_Receive_IT ( & SIM808_UART, & SIM808_rx_byte, 1) != HAL_OK)
	{
		while (1)
			;
	}
	while (SIM808_GetAT (10 * 1000) != SIM_OK)
	{
		HAL_Delay (20);
	}
	while (SIM808_SetEchoOff (10 * 1000) != SIM_OK)
	{
		HAL_Delay (20);
	}
//	SIM808_Dial (20 * 1000);
	SIM808_SendSMS (20 * 1000, "From STM32");
}

/*
 * @brief	Sends AT command to SIM808 and waits for resp1 or resp2
 * @note	This function is blocking
 * @retval	SIM_OK if successful transfer, otherwise SIM_ER
 */
SIM808Status SIM808_SendATCommand (char* cmd, char* resp1, char* resp2, uint32_t timeout_ms)
{
	uint32_t ii = 0;
	uint32_t resp1_len = strlen (resp1);
	uint32_t resp2_len = 0;
	uint8_t flag_resp1_ok = 1;
	uint32_t init_time = HAL_GetTick ();

	if (resp2 != NULL)
	{
		resp2_len = strlen (resp2);
	}

//	Used to wake up the SIM808 when in mode 2
	SIM808_SendString ("AT\r");
	HAL_Delay (300);

	SIM808_rx_head = 0; // reset buffer
	SIM808_SendString (cmd);

//	 wait for the response
	while (1)
	{
		if (SIM808_rx_head > ii)
		{
			if (flag_resp1_ok == 0 || SIM808_rx_buf[ii] != resp1[ii])
			{
				flag_resp1_ok = 0;
				if (resp2 != NULL)
				{
					if (SIM808_rx_buf[ii] != resp2[ii])
					{
						return SIM_ER;
					}
				}
				else
				{
					return SIM_ER;
				}
			}
			ii++;
			if ( (ii >= resp1_len && flag_resp1_ok == 1)
					|| (resp2 != NULL && ii >= resp2_len && flag_resp1_ok == 0))
			{
				return SIM_OK;
			}
		}
		else
		{
			if (init_time + timeout_ms < HAL_GetTick ())
			{
				return SIM_ER;
			}
			HAL_Delay (5);
		}

	}

	return SIM_ER;
}
/*
 * @brief	Sends input cmd over UART by the DMA. Input string must be null terminated.
 */
void SIM808_SendString (char* cmd)
{
	HAL_UART_Transmit_DMA ( & SIM808_UART, (uint8_t *) cmd, strlen (cmd));
}

//--------------------------------------------------------------------------------------

SIM808Status SIM808_GetAT (uint32_t timeout_ms)
{
	return SIM808_SendATCommand ("AT\r", "\r\nOK\r\n", "AT\r\r\nOK\r\n", timeout_ms);
}
SIM808Status SIM808_SetEchoOff (uint32_t timeout_ms)
{
	return SIM808_SendATCommand ("ATE0\r", "\r\nOK\r\n", "ATE0\r\r\nOK\r\n", timeout_ms);
}
SIM808Status SIM808_SetPowerSaving (uint32_t timeout_ms)
{
	return SIM808_SendATCommand ("AT+CSCLK=2\r", "\r\nOK\r\n", "\r\nERROR\r\n", timeout_ms);
}

SIM808Status SIM808_Dial (uint32_t timeout_ms)
{
	char tempStr[64] = "ATD";
	strlcat (tempStr, GSM_contact, sizeof (tempStr));
	strlcat (tempStr, ";\r", sizeof (tempStr));
	SIM808_SendString (tempStr);
	HAL_Delay (10 * 1000);
	SIM808_SendString ("ATH\r");
	return SIM_OK;
}

SIM808Status SIM808_SendSMS (uint32_t timeout_ms, char* msg)
{
	char tempStr[512] = "AT+CMGS=\"";
	strlcat (tempStr, GSM_contact, sizeof (tempStr));
	strlcat (tempStr, "\"\r", sizeof (tempStr));

	if (SIM808_SendATCommand ("AT+CMGF=1\r", "\r\nOK\r\n", NULL, timeout_ms) == SIM_OK)
	{
		if (SIM808_SendATCommand (tempStr, "\r\n> ", NULL, timeout_ms) == SIM_OK)
		{
			tempStr[0] = '\0';
			strlcat (tempStr, msg, sizeof (tempStr));
			strlcat (tempStr, "\x1A", sizeof (tempStr));
			SIM808_SendString (tempStr);
			HAL_Delay (1000);
			SIM808_SendString ("AT+CMGF=0\r");
			HAL_Delay (100);
			return SIM_OK;
		}
	}
	return SIM_ER;
}

SIM808Status SIM808_SetGNSSOn (uint32_t timeout_ms)
{
	return SIM808_SendATCommand ("AT+CGNSPWR=1\r", "\r\nOK\r\n", NULL, timeout_ms);
}

SIM808Status SIM808_SetRMSFormat (uint32_t timeout_ms)
{
	return SIM808_SendATCommand ("AT+CGNSSEQ=\"RMC\"\r", "\r\nOK\r\n", NULL, timeout_ms);
}
SIM808Status SIM808_GetGNSSLocation (uint32_t timeout_ms)
{
	SIM808_SendString("AT+CGNSINF\r");
	// TODO wait until timeout or until OK
	// if OK parse GNSS data into struct
}
