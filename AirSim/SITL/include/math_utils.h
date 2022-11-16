#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <array>
#include <vector>
#include <Eigen/Dense>
#define Matrix Eigen::Matrix

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958
#define MILLI2BASE 0.001

#define M_PI 3.14159265358979323846
#define GRAVITY 9.81

#define MOTOR_NUM 4
#define MIN_PWM (uint16_t)1000
#define MAX_PWM (uint16_t)2000
#define MIN_MOTOR_CMD 0.0f
#define MAX_MOTOR_CMD 1.0f

using state_t = Matrix<float, 16, 1>;
using quat_t = Matrix<float, 4, 1>;
using vec_t = Matrix<float, 3, 1>;
using mat3x3_t = Matrix<float, 3, 3>;
using mat4x4_t = Matrix<float, 4, 4>;
using euler_t = Matrix<float, 3, 1>;
using array_t = std::array<float, 3>;
using zero_t = std::vector<float>;
using act_t = std::array<uint16_t, MOTOR_NUM>;
using motor_t = std::array<float, MOTOR_NUM>;

mat3x3_t rotation_from_quaternion( const quat_t& );
void normalize_quaternion( quat_t& );
void quat2euler( const quat_t& q, float& phi, float& theta, float& psi );

template <typename T>
T fisrt_order_euler_integration( T state, T derivative, const float& dt )
{
  return state + derivative * dt;
}

template <typename T>
T second_order_euler_integration( T state, T derivative1, T derivative2, const float& dt )
{
  return state + ( derivative1 + derivative2 ) * ( dt * 0.5 );
}

template <typename T_in, typename T_out>
T_out linear_map( T_in x, T_in in_min, T_in in_max, T_out out_min, T_out out_max ) {
  return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min;
}

#endif