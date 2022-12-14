#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "math_utils.h"

#define P_ROLL_ANGLE 0.9
#define P_PITCH_ANGLE 0.9

#define P_ROLL_RATE 0.12
#define P_PITCH_RATE 0.12
#define P_YAW_RATE 5.0

#define D_ROLL_RATE 1.0
#define D_PITCH_RATE 1.0

#define FF_ROLL 2
#define FF_PITCH 2
#define FF_YAW 2

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

  float last_rate[3];
};

#endif
