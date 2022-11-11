#include "sensors.h"
#include "navigation.h"

unsigned long nav_previousTime = 0;
const long nav_interval = 10;

Sensors sens;
Navigation nav;

void setup() {
  Serial.begin(115200);
  
  sens.init();
  nav.init();

  delay(100);  
}

void loop() {
  unsigned long currentTime = millis();

  if(currentTime - nav_previousTime >= nav_interval) 
  {
    nav_previousTime = currentTime;
    sens.update();
    // sens.print();

    float dt = nav_interval * 0.001;
    nav.update(sens, dt);
    nav.print();
  }
}
