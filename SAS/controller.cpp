#include "controller.h"

Controller::Controller()
{
	for ( auto i = 0; i < MOTOR_NUM; i++ )
	{
		this->pwm_out[i] = MIN_PWM_OUT;
	}

  thr_out = 0;
  roll_out = 0;
  pitch_out = 0;
  yaw_out = 0;
}

Controller::~Controller()
{
}

void Controller::init()
{

}

void Controller::update(const sens_t& sens, const state_t& state, const guidance_t& cmd)
{
  this->attitude_controller(sens, cmd);
  this->altitude_controller(cmd);
}

void Controller::attitude_controller(const sens_t& sens, const guidance_t& cmd)
{
  // attitude and rate control for roll and pitch
  this->roll_out = K_ROLL_ANGLE*(cmd.ROLL - sens.euler[0]) + K_ROLL_RATE*(cmd.ROLL_RATE - sens.gyr[0]);
  this->pitch_out = K_PITCH_ANGLE*(cmd.PITCH - sens.euler[1]) + K_PITCH_RATE*(cmd.PITCH_RATE - sens.gyr[1]);

  // only rate control for yaw
  this->yaw_out = K_YAW_RATE*(cmd.YAW_RATE - sens.gyr[2]);
}

void Controller::altitude_controller(const guidance_t& cmd)
{
  // throttle is passthrough since no altitude control (yet)
  this->thr_out = cmd.THR;
}

void Controller::print()
{

}