#ifndef SENSORS_H
#define SENSORS_H

#include "math_utils.h"

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
};

#endif