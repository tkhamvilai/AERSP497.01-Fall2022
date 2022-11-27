#include "controller.h"

Controller::Controller()
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->pwm_out[i] = MIN_PWM_OUT;
	}
}

Controller::~Controller()
{
}

void Controller::init()
{

}

void Controller::update(const sens_t& sens, const state_t& state, const guidance_t& cmd)
{

}

void Controller::control_algorithm(const sens_t& sens, const state_t& state)
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->pwm_out[i] = MAX_PWM_OUT;
	}
}

void Controller::print()
{

}