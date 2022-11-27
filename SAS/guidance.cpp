#include "guidance.h"

Guidance::Guidance()
{
  this->cmd.THR = 0;
  this->cmd.ROLL = 0;
  this->cmd.PITCH = 0;
  this->cmd.YAW = 0;
  this->cmd.ROLL_RATE = 0;
  this->cmd.PITCH_RATE = 0;
  this->cmd.YAW_RATE = 0;
}

Guidance::~Guidance()
{
}

void Guidance::init()
{

}

void Guidance::update(const sens_t& sens, const state_t& state, const rc_t& rc)
{
  this->cmd.THR = rc.THR; // throttle is passthrough since no altitude control (yet)
  this->cmd.ROLL = linear_map<float, int16_t>(rc.ROLL, rc.ROLL_MIN, rc.ROLL_MAX, -ROLL_ANGLE_LIMIT, ROLL_ANGLE_LIMIT);
  this->cmd.PITCH = linear_map<float, int16_t>(rc.PITCH, rc.PITCH_MIN, rc.PITCH_MAX, -PITCH_ANGLE_LIMIT, PITCH_ANGLE_LIMIT);
  this->cmd.YAW_RATE = linear_map<float, int16_t>(rc.YAW, rc.YAW_MIN, rc.YAW_MAX, -YAW_RATE_LIMIT, YAW_RATE_LIMIT);
}

void Guidance::print()
{

}