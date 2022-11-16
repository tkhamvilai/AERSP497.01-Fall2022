#include "controller.h"

Controller::Controller()
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->pwm_out[i] = MIN_PWM;
	}
}

Controller::~Controller()
{
}

void Controller::control_algorithm( state_t state, sens_t sens)
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->pwm_out[i] = MAX_PWM;
	}
}
