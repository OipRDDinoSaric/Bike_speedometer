#include "HALSensor.h"

uint32_t timestamps[BUFFER_SIZE];
//velocity in cm/s
double vel[BUFFER_SIZE];
static uint32_t head;
static uint32_t prev;

static uint32_t start;

void HALSensor_init (void)
{
	HAL_TIM_IC_Start_IT ( & htim2, TIM_CHANNEL_2);
}

void HALSensor_push (uint32_t value)
{
	double T;
	start = HAL_GetTick ();
	prev = head;
	head++;
	head = head >= BUFFER_SIZE ? 0: head;
	timestamps[head] = value;

	T = (timestamps[head] - timestamps[prev]) / 20000.; // 20000 bcs we convert to seconds
	if (T != 0)
		vel[prev] = (double) (2 * PI * (double) (RADIUS_CM) / T);
	else
		vel[prev] = 0;
}

double HALSensor_GetVelocityKmph (void)
{
	double velocity = 0;
	int it;

	if (HAL_GetTick () - start > 1400)
	{
		for (it = 0; it < BUFFER_SIZE; ++it)
			vel[it] = 0;
		return 0.;
	}
	for (it = 0; it < BUFFER_SIZE; ++it)
	{
		velocity += vel[it];
	}

	return (double) (velocity * 0.036 / (double) BUFFER_SIZE);
}
