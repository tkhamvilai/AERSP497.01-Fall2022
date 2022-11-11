#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <BasicLinearAlgebra.h>
#define Matrix BLA::Matrix

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958
#define MILLI2BASE 0.001

#define POZYX_GYR_SCALE 0.0625
#define POZYX_MAG_SCALE 0.0625
#define POZYX_EULER_SCALE 0.0625
#define POZYX_QUAT_SCALE 1.0/16384.0

#define GRAVITY 9.81

using state_t = Matrix<16, 1>;
using quat_t = Matrix<4, 1>;
using vec_t = Matrix<3, 1>;
using mat3x3_t = Matrix<3, 3>;
using mat4x4_t = Matrix<4, 4>;
using euler_t = Matrix<3, 1>;

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