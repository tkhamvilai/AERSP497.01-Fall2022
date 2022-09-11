#define LED1_PIN 5
#define LED2_PIN 6
#define LED3_PIN 7

#define LED1_INTERVAL 200
#define LED2_INTERVAL 400
#define LED3_INTERVAL 600

unsigned long LED1_previousMillis = 0;
unsigned long LED2_previousMillis = 0;
unsigned long LED3_previousMillis = 0;

void setup() {
  DDRD |= ( (1 << PD5) | (1 << PD6) | (1 << PD7) );
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - LED1_previousMillis >= LED1_INTERVAL) {
    LED1_previousMillis = currentMillis;
    int val = (PIND & (1 << PD5)) >> 5;
    PORTD = (val) ? ( PORTD & ~(1 << PD5) ) : ( PORTD | (1 << PD5) );
  }

  if (currentMillis - LED2_previousMillis >= LED2_INTERVAL) {
    LED2_previousMillis = currentMillis;
    int val = (PIND & (1 << PD6)) >> 6;
    PORTD = (val) ? ( PORTD & ~(1 << PD6) ) : ( PORTD | (1 << PD6) );
  }

  if (currentMillis - LED3_previousMillis >= LED3_INTERVAL) {
    LED3_previousMillis = currentMillis;
    int val = (PIND & (1 << PD7)) >> 7;
    PORTD = (val) ? ( PORTD & ~(1 << PD7) ) : ( PORTD | (1 << PD7) );
  }
}
