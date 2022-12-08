#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "math_utils.h"

#define K_ROLL_ANGLE 1.0
#define K_PITCH_ANGLE 1.0

#define K_ROLL_RATE 10.0
#define K_PITCH_RATE 10.0
#define K_YAW_RATE 10.0

class Controller
{
public:
	Controller();
	~Controller();

	void init();
  void update(const sens_t&, const state_t&, const guidance_t&);
  void print();

	uint16_t pwm_out[MOTOR_NUM];

  float thr_out;
  float roll_out;
  float pitch_out;
  float yaw_out;

private:
  void attitude_controller(const sens_t&, const guidance_t&);
  void altitude_controller(const guidance_t&);
  void mixer();
};

#endif
