#ifndef GUIDANCE_H
#define GUIDANCE_H

#include "sensors.h"
#include "navigation.h"
#include "receiver.h"
#include "math_utils.h"

class Guidance
{
public:
	Guidance();
	~Guidance();

	void init();
  void update(const sens_t&, const state_t&, const rc_t&);
  void print();

	guidance_t cmd;

private:

};

#endif