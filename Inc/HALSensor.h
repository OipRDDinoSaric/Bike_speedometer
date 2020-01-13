#include "main.h"

extern TIM_HandleTypeDef htim2;

#define RADIUS_CM 15

#define PI 3.14159
#define BUFFER_SIZE 5

double HALSensor_GetVelocityKmph(void);
void HALSensor_push(uint32_t value);
void HALSensor_init(void);
