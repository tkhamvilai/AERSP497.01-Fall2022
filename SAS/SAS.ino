#include "sensors.h"
#include "navigation.h"

unsigned long previousTime_1 = 0;
const long interval_1 = 10; // millisecond

unsigned long previousTime_2 = 0;
const long interval_2 = 20; // millisecond

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

  if(currentTime - previousTime_1 >= interval_1) 
  {
    // Serial.println(previousTime_1);
    previousTime_1 = currentTime;
    float dt = interval_1 * 0.001; // convert to second

    sens.update();
    // sens.print();
    
    // nav.update(sens, dt);
    // nav.print();
  }
}
