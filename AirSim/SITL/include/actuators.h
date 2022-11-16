#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "math_utils.h"

class Actuators
{
public:
	Actuators();
	~Actuators();

	void set_pwm( act_t pwm );
	virtual void drive_motors() = 0;

protected:
	motor_t motor_cmd;

};

#endif
