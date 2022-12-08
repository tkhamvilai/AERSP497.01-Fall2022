#ifndef LED_H
#define LED_H

class LED
{
public:
  LED();
  ~LED();

  void init();
  void on();
  void off();
  void toggle();
  
private:
  int led_pin = 13;
};

#endif
