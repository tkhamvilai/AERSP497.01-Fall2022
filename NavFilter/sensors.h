#ifndef SENSORS_H
#define SENSORS_H

class Sensors
{
public:
  Sensors();
  ~Sensors();

  void init();
  void update();
  void print();

  float gyr[3];
  float acc[3];
  float mag[3];

private:
};

#endif