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

void Navigation::update(const Sensors& sens)
{
  this->process_model(sens);
}

void Navigation::process_model(const Sensors& sens)
{
  quat_t q = {this->s(0), this->s(1), this->s(2), this->s(3)}; // quaternion
  vec_t bw = {this->s(13), this->s(14), this->s(15)}; // gyro bias

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

  vec_t a = {sens.acc[0], sens.acc[1], sens.acc[2]}; // acceleration
  vec_t ba = {this->s(10), this->s(11), this->s(12)}; // acc bias
  vec_t g = {0, 0, -9.81};
  mat3x3_t T = this->rotation_from_quaternion(q);

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
}

mat3x3_t Navigation::rotation_from_quaternion(const quat_t& q)
{
  mat3x3_t T;
  
  T(0,0) = 1-2*(q(2)*q(2)+q(3)*q(3));
  T(0,1) = 2*(q(1)*q(2)-q(0)*q(3));
  T(0,2) = 2*(q(1)*q(3)+q(0)*q(2));

  T(1,0) = 2*(q(1)*q(2)+q(0)*q(3));
  T(1,1) = 1-2*(q(1)*q(1)+q(3)*q(3));
  T(1,2) = 2*(q(2)*q(3)-q(0)*q(1));

  T(2,0) = 2*(q(1)*q(3)-q(0)*q(2));
  T(2,1) = 2*(q(2)*q(3)-q(0)*q(1));
  T(2,2) = 1-2*(q(1)*q(1)+q(2)*q(2));

  return T;
}

void Navigation::print()
{
  Serial << this->s;
  Serial.println();
  Serial << this->s_dot;
  Serial.println();
}