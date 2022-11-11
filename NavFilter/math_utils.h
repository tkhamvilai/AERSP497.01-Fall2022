#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <BasicLinearAlgebra.h>

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958

using state_t = BLA::Matrix<16, 1>;
using quat_t = BLA::Matrix<4, 1>;
using vec_t = BLA::Matrix<3, 1>;
using mat3x3_t = BLA::Matrix<3, 3>;
using mat4x4_t = BLA::Matrix<4, 4>;
using euler_t = BLA::Matrix<3, 1>;

mat3x3_t rotation_from_quaternion(const quat_t&);
void normalize_quaternion(quat_t&);
void quat2euler( const quat_t& q, float& phi, float& theta, float& psi );

template <typename T>
T euler_integration(T state, T derivative, const float& dt)
{
  return state + derivative * dt;
}

#endif