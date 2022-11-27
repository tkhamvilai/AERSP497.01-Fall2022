#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "math_utils.h"

class Actuators
{
public:
  Actuators();
  ~Actuators();

  void init();
  void calibrate();
  void stop();
  void update(uint16_t[MOTOR_NUM]);
  void print();

private:

};

#endif