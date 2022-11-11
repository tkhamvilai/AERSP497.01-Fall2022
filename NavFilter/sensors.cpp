#include "sensors.h"

#include <Pozyx.h>
#include <Pozyx_definitions.h>
#include <Wire.h>

Sensors::Sensors()
{
  for(uint8_t i = 0; i < 3; i++)
  {
    this->gyr[i] = 0;
    this->acc[i] = 0;
    this->mag[i] = 0;
  }
}

Sensors::~Sensors() {}

void Sensors::init()
{
  if(Pozyx.begin(false, MODE_INTERRUPT, POZYX_INT_MASK_IMU) == POZYX_FAILURE)
  {
    Serial.println("ERROR: Unable to connect to POZYX shield");
    Serial.println("Reset required");
    delay(100);
    abort();
  }
  this->update();
}

void Sensors::update()
{
  sensor_raw_t sensor_raw;
  if (Pozyx.waitForFlag(POZYX_INT_STATUS_IMU, 10) == POZYX_SUCCESS){
    Pozyx.getRawSensorData(&sensor_raw);
  }else{
    uint8_t interrupt_status = 0;
    Pozyx.getInterruptStatus(&interrupt_status);
    return;
  }

  for(uint8_t i = 0; i < 3; i++)
  {
    this->gyr[i] = sensor_raw.angular_vel[i] * DEG2RAD * POZYX_GYR_SCALE; // convert to rad/s
    this->acc[i] = sensor_raw.acceleration[i] * MILLI2BASE * GRAVITY; // convert from milli-g to m/s^2
    this->mag[i] = sensor_raw.magnetic[i] * POZYX_MAG_SCALE; // convert to µT
    if(i > 0) // convert to NED
    {
      this->gyr[i] *= -1.0; 
      this->acc[i] *= -1.0; 
      this->mag[i] *= -1.0; 
    }
  }
}

void Sensors::print()
{
  Serial.print(this->gyr[0]);
  Serial.print(",");
  Serial.print(this->gyr[1]);
  Serial.print(",");
  Serial.print(this->gyr[2]);
  Serial.print(",");
  Serial.print(this->acc[0]);
  Serial.print(",");
  Serial.print(this->acc[1]);
  Serial.print(",");
  Serial.print(this->acc[2]);
  Serial.print(",");
  Serial.print(this->mag[0]);
  Serial.print(",");
  Serial.print(this->mag[1]);
  Serial.print(",");
  Serial.print(this->mag[2]);
  Serial.println();
}