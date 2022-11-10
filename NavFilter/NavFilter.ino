#include "sensors.h"
#include "navigation.h"

uint32_t last_millis;

Sensors sens;
Navigation nav;

void setup() {
  Serial.begin(115200);
  
  sens.init();
  nav.init();

  last_millis = millis();
  delay(10);  
}

void loop() {
  sens.update();
  // sens.print();

  nav.update(sens);
  nav.print();
  while(1);
}
