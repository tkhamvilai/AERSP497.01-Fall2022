#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "sensors.h"

#include <BasicLinearAlgebra.h>
using state_t = BLA::Matrix<16, 1>;
using quat_t = BLA::Matrix<4, 1>;
using vec_t = BLA::Matrix<3, 1>;
using mat3x3_t = BLA::Matrix<3, 3>;
using mat4x4_t = BLA::Matrix<4, 4>;

/*
  state = [q0,q1,q2,q3,x,y,z,u,v,w,bax,bay,baz,bwx,bwy,bwz]
*/

class Navigation
{
public:
  Navigation();
  ~Navigation();

  void init();
  void update(const Sensors&);

  mat3x3_t rotation_from_quaternion(const quat_t&);

  void print();

  state_t s;
  state_t s_dot;

private:
  void process_model(const Sensors&);
};

#endif