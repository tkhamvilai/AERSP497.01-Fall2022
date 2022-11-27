#ifndef RECEIVER_H
#define RECEIVER_H

#include "Arduino.h"
#include "math_utils.h"

#define RC_CHANS 6
#define PCINT_PIN_COUNT 6
#define PCINT_RX_BITS (1<<2),(1<<4),(1<<5),(1<<6),(1<<7),(1<<0)
#define RX_PCINT_PIN_PORT PIND
#define RX_PCINT_PIN_PORT2 PINB

static uint8_t PCInt_RX_Pins[PCINT_PIN_COUNT] = {PCINT_RX_BITS};
static volatile uint16_t rcValue[RC_CHANS];
static uint16_t edgeTime[RC_CHANS];

class Receiver
{
public:
  Receiver();
  ~Receiver();

  void init();
  void update();
  void print();

  rc_t rc_in;

private:
  int16_t rcData[RC_CHANS];

  uint16_t readRawRC(uint8_t chan);
  void mapRCcmd();
};

#endif