#define LED_NUM 3

int LED_PINS[LED_NUM] = {5,6,7};
int LED_INTERVALS[LED_NUM] = {200,400,600};
unsigned long LED_previousMillis[LED_NUM] = {0,0,0};

void blink(int LED_PIN);

void setup() {
  for(uint8_t i = 0; i < LED_NUM; i++) {
    DDRD |= (1 << LED_PINS[i]);
  }
}

void loop() {
  for(uint8_t i = 0; i < LED_NUM; i++) {
    blink(i);
  }
}

void blink(int LED_PIN)
{
  unsigned long currentMillis = millis();

  if (currentMillis - LED_previousMillis[LED_PIN] >= LED_INTERVALS[LED_PIN]) {
    LED_previousMillis[LED_PIN] = currentMillis;
    int val = (PIND & (1 << LED_PINS[LED_PIN])) >> LED_PINS[LED_PIN];
    PORTD = (val) ? ( PORTD & ~(1 << LED_PINS[LED_PIN]) ) : ( PORTD | (1 << LED_PINS[LED_PIN]) );
  }
}
