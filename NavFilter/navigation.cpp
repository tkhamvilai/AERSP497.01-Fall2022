#include "navigation.h"

Navigation::Navigation()
{
  this->s.Fill(0);
  this->s_dot.Fill(0);
}

Navigation::~Navigation() {}

void Navigation::init()
{
  this->s(0) = 1; // q0
}

void Navigation::update(const Sensors& sens, const float& dt)
{
  // state propagation
  this->process_model(sens);
  this->s_predicted = euler_integration<state_t>(s, s_dot, dt);

  // covariance propagation

  // compute Kalman gain

  // measurement update
  this->s = this->s_predicted;

  // covariance update
}

void Navigation::process_model(const Sensors& sens)
{
  quat_t q = {this->s(0), this->s(1), this->s(2), this->s(3)}; // quaternion
  vec_t bw = {this->s(13), this->s(14), this->s(15)}; // gyro bias
  vec_t a = {sens.acc[0], sens.acc[1], sens.acc[2]}; // acceleration
  vec_t ba = {this->s(10), this->s(11), this->s(12)}; // acc bias
  vec_t g = {0, 0, -9.81};

  normalize_quaternion(q);
  mat3x3_t T = rotation_from_quaternion(q);

  mat4x4_t f;
  f.Fill(0);
  f(0,1) = -(sens.gyr[0] - bw(0)) * 0.5;
  f(0,2) = -(sens.gyr[1] - bw(1)) * 0.5;
  f(0,3) = -(sens.gyr[2] - bw(2)) * 0.5;

  f(1,0) =  (sens.gyr[0] - bw(0)) * 0.5;
  f(1,2) =  (sens.gyr[2] - bw(2)) * 0.5;
  f(1,3) = -(sens.gyr[1] - bw(1)) * 0.5;

  f(2,0) =  (sens.gyr[1] - bw(1)) * 0.5;
  f(2,1) = -(sens.gyr[2] - bw(2)) * 0.5;
  f(2,3) =  (sens.gyr[0] - bw(0)) * 0.5;

  f(3,0) =  (sens.gyr[2] - bw(2)) * 0.5;
  f(3,1) =  (sens.gyr[1] - bw(1)) * 0.5;
  f(3,2) = -(sens.gyr[0] - bw(0)) * 0.5;

  quat_t q_dot = f * q;
  vec_t v_dot = T * (a - ba) + g;

  this->s_dot(0) = q_dot(0);
  this->s_dot(1) = q_dot(1);
  this->s_dot(2) = q_dot(2);
  this->s_dot(3) = q_dot(3);
  this->s_dot(4) = this->s(7);
  this->s_dot(5) = this->s(8);
  this->s_dot(6) = this->s(9);
  this->s_dot(7) = v_dot(0);
  this->s_dot(8) = v_dot(1);
  this->s_dot(9) = v_dot(2);

  quat2euler(q, this->angles(0), this->angles(1), this->angles(2));
}

void Navigation::print()
{
  // Serial.print("s: ");
  // Serial << this->s;
  // Serial.println();
  // Serial.print("s_dot: ");
  // Serial << this->s_dot;
  // Serial.println();

  Serial << this->angles * RAD2DEG;
  Serial.println();
}