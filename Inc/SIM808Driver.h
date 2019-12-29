#include "main.h"
#include "cmsis_os.h"
#include <string.h>

extern UART_HandleTypeDef huart2;
#define SIM808_UART huart2

#define SIM808_USEGNSS	1
#define SIM808_RX_SIZE 512

extern volatile uint32_t SIM808_rx_head;
extern volatile uint8_t SIM808_rx_buf[SIM808_RX_SIZE];
extern volatile uint8_t SIM808_rx_byte;
extern volatile uint8_t SIM808_flag_rx_irq;

typedef struct
{
	uint8_t state;
} SIM808Driver;

typedef enum SIM808Status_TypeDef
{
	SIM_OK = 0, SIM_ER = 1
} SIM808Status;

void SIM808_Init (void);
SIM808Status SIM808_SendATCommand (char* cmd, char* resp1, char* resp2, uint32_t timeout_ms);
void SIM808_SendString (char* cmd);

SIM808Status SIM808_GetAT (uint32_t timeout_ms);
SIM808Status SIM808_SetEchoOff (uint32_t timeout_ms);
SIM808Status SIM808_SetPowerSaving (uint32_t timeout_ms);
SIM808Status SIM808_Dial(uint32_t timeout_ms);
SIM808Status SIM808_SendSMS (uint32_t timeout_ms, char* msg);
