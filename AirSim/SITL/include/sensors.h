#ifndef SENSORS_H
#define SENSORS_H

#include "math_utils.h"

struct sens_t
{
  array_t gyr;
  array_t acc;
  array_t mag;
  float baro_pressure;
};

class Sensors
{
public:
  Sensors();
  ~Sensors();

  virtual void sensor_read() = 0;
  void sensor_print();

  sens_t sens;

private:
};

#endif