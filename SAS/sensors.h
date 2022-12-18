#ifndef SENSORS_H
#define SENSORS_H

#include "math_utils.h"

#define NUM_CALIBRATION 500
#define LOWPASS_WEIGHT 0.9

class Sensors
{
public:
  Sensors();
  ~Sensors();

  void init();
  void update();
  void print();

  sens_t data;

private:
  sens_t bias;
  bool calibration_flag;
};

#endif