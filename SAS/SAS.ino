#include "sensors.h"
#include "navigation.h"
#include "receiver.h"
#include "controller.h"
#include "actuators.h"
#include "guidance.h"
#include "led.h"

unsigned long previousTime = 0;
const long interval = 1; // millisecond

Sensors sens;
Navigation nav;
Receiver rc;
Guidance gd;
Controller cntrl;
Actuators motors;
LED led;

void setup() {  
  led.init();
  motors.init();
  sens.init();
  nav.init();
  rc.init();
  gd.init();
  cntrl.init();

  Serial.begin(115200);
  delay(3000);

  do
  {
    rc.update();
    motors.stop();
    delay(100);
  }
  while(rc.rc_in.AUX2 >= ARM_DISARM_PWM_THRESHOLD);    
}

void loop() {
  // unsigned long currentTime = millis();

  // if(currentTime - previousTime >= interval) 
  // {
    // Serial.println(previousTime);
    // previousTime = currentTime;
    // float dt = interval * 0.001; // convert to second

    sens.update();
    // nav.update(sens.data, dt);
    rc.update();
    gd.update(sens.data, nav.s, rc.rc_in);
    cntrl.update(sens.data, nav.s, gd.cmd);

    if(rc.rc_in.AUX2 < ARM_DISARM_PWM_THRESHOLD){
      motors.stop();
      led.off();
    }
    else{
      motors.update(cntrl.pwm_out);
      led.toggle();
    }

    print();
  // }
}

void print()
{
//  sens.print();
//  nav.print();
//  rc.print();
//  gd.print();
//  cntrl.print();
//  motors.print();
}
