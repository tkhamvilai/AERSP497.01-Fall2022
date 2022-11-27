#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "math_utils.h"

class Controller
{
public:
	Controller();
	~Controller();

	void init();
  void update(const sens_t&, const state_t&, const guidance_t&);
  void print();

	uint16_t pwm_out[MOTOR_NUM];

private:
  void control_algorithm(const sens_t&, const state_t&);
};

#endif