#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "navigation.h"

class Controller
{
public:
	Controller();
	~Controller();

	void control_algorithm(state_t, sens_t);

	act_t pwm_out;

private:

};

#endif
