#include "led.h"
#include "Arduino.h"

LED::LED()
{
  
}

LED::~LED()
{
  
}

void LED::init()
{
  pinMode(this->led_pin, OUTPUT);
  this->on();
}

void LED::on()
{
  digitalWrite(this->led_pin, HIGH);
}

void LED::off()
{
  digitalWrite(this->led_pin, LOW);
}

void LED::toggle()
{
  digitalWrite(this->led_pin, !digitalRead(this->led_pin));
}
