#include "actuators.h"

Actuators::Actuators()
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->motor_cmd[i] = MIN_MOTOR_CMD;
	}
}

Actuators::~Actuators()
{
}

void Actuators::set_pwm( act_t pwm )
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->motor_cmd[i] = linear_map<decltype( +pwm [0] ), decltype( +motor_cmd[0] )>(pwm[i], MIN_PWM, MAX_PWM, MIN_MOTOR_CMD, MAX_MOTOR_CMD);
	}
}