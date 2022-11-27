#include "sensors.h"
#include "navigation.h"
#include "receiver.h"
#include "controller.h"
#include "actuators.h"
#include "guidance.h"

unsigned long previousTime = 0;
const long interval = 2; // millisecond

Sensors sens;
Navigation nav;
Receiver rc;
Guidance gd;
Controller cntrl;
Actuators motors;

void setup() {
  Serial.begin(115200);
  
  sens.init();
  nav.init();
  rc.init();
  gd.init();
  cntrl.init();
  motors.init();

  motors.calibrate();
  motors.stop();

  delay(3000);

  do
  {
    rc.update();
    motors.stop();
  }
  while(rc.rc_in.AUX2 >= ARM_DISARM_PWM_THRESHOLD);    
}

void loop() {
  unsigned long currentTime = millis();

  if(currentTime - previousTime >= interval) 
  {
    // Serial.println(previousTime);
    previousTime = currentTime;
    float dt = interval * 0.001; // convert to second

    sens.update();
    // sens.print();

    // nav.update(sens.data, dt);
    // nav.print();

    rc.update();
    // rc.print();

    gd.update(sens.data, nav.s, rc.rc_in);
    // gd.print();

    cntrl.update(sens.data, nav.s, gd.cmd);
    // cntrl.print();

    if(rc.rc_in.AUX2 < ARM_DISARM_PWM_THRESHOLD)
      motors.stop();
    else
      motors.update(cntrl.pwm_out);
    // motors.print();
  }
}
