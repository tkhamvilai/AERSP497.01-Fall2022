#ifndef AIRSIM_H
#define AIRSIM_H

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "math_utils.h"
#include "sensors.h"
#include "actuators.h"

struct true_state_t
{  
  vec_t position;
  quat_t orientation;

  vec_t linear_velocity;
  vec_t angular_velocity;

  vec_t linear_acceleration;
  vec_t angular_acceleration;
};

class AirSim : public Sensors, public Actuators
{
public:
	AirSim();
	~AirSim();

  void arm();
  void disarm();

  void sensor_read();
  void get_truth();

  void drive_motors();

	msr::airlib::MultirotorRpcLibClient client;
  true_state_t true_states;

private:
};

#endif

