#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <BasicLinearAlgebra.h>
#define Matrix BLA::Matrix

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958
#define MILLI2BASE 0.001
#define GRAVITY 9.81

#define MOTOR_NUM 4
#define MIN_DUTY_CYCLE 125
#define MAX_DUTY_CYCLE 255
#define MIN_PWM_OUT 1000
#define MAX_PWM_OUT 2000
#define MIN_PWM_IN 1000
#define MAX_PWM_IN 2000
#define ROLL_MID_PWM 1500
#define PITCH_MID_PWM 1500
#define YAW_MID_PWM 1500
#define ARM_DISARM_PWM_THRESHOLD 1500

using state_t = Matrix<16, 1>;
using quat_t = Matrix<4, 1>;
using vec_t = Matrix<3, 1>;
using mat3x3_t = Matrix<3, 3>;
using mat4x4_t = Matrix<4, 4>;
using euler_t = Matrix<3, 1>;

struct sens_t
{
  float gyr[3];
  float acc[3];
  float mag[3];
  float euler[3];
  float quat[4];
};

struct rc_t
{
  int16_t ROLL;
  int16_t PITCH;
  int16_t THR;
  int16_t YAW;
  int16_t AUX;
  int16_t AUX2;
};

struct guidance_t
{
  int16_t THR;
  int16_t ROLL;
  int16_t PITCH;
  int16_t YAW;
  int16_t ROLL_RATE;
  int16_t PITCH_RATE;
  int16_t YAW_RATE;
};

enum motor 
{
  FRONT_RIGHT,
  FRONT_LEFT,
  REAR_LEFT,
  REAR_RIGHT
};

enum rc 
{
  ROLL,
  PITCH,
  THR,
  YAW,
  AUX,
  AUX2
};

mat3x3_t rotation_from_quaternion(const quat_t&);
void normalize_quaternion(quat_t&);
void quat2euler( const quat_t& q, float& phi, float& theta, float& psi );

template <typename T>
T fisrt_order_euler_integration(T state, T derivative, const float& dt)
{
  return state + derivative * dt;
}

template <typename T>
T second_order_euler_integration(T state, T derivative1, T derivative2, const float& dt)
{
  return state + (derivative1 + derivative2) * (dt * 0.5);
}

#endif