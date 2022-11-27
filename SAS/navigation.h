#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "sensors.h"
#include "math_utils.h"

/*
  state = [q0,q1,q2,q3,x,y,z,u,v,w,bax,bay,baz,bwx,bwy,bwz]
*/

class Navigation
{
public:
  Navigation();
  ~Navigation();

  void init();
  void update(const Sensors&, const float&);

  void print();

  state_t s; // x^hat+
  state_t s_dot; // x^hat_dot
  state_t s_predicted; // x^hat-
  
  euler_t angles;

private:
  void process_model(const Sensors&, const state_t&, state_t&);
};

#endif