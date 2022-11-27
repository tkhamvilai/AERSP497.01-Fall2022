#ifndef SENSORS_H
#define SENSORS_H

struct sens_t
{
  float gyr[3];
  float acc[3];
  float mag[3];
  float euler[3];
  float quat[4];
};

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