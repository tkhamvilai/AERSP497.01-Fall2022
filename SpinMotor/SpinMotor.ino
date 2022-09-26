void setup() {
  // set PORTD (D3) as an OUTPUT
  DDRD |= (1 << DDD3);
  // set PORTB (D11) as an OUTPUT
  DDRB |= (1 << DDB3);

  // set Compare Output Mode for OC2B
  TCCR2A |= (1 << COM2B1); TCCR2A &= ~(1 << COM2B0);

  // set Compare Output Mode for OC2A
  TCCR2A |= (1 << COM2A1); TCCR2A &= ~(1 << COM2A0);

  // set PWM generation mode to Phase Correct
  TCCR2B &= ~(1 << WGM22); TCCR2A &= ~(1 << WGM21); TCCR2A |= (1 << WGM20);
  
  // set prescaling factor to 64
  TCCR2B |= (1 << CS22); TCCR2B &= ~(1 << CS21); TCCR2B &= ~(1 << CS20);

  // wait to make sure everything is set
  delay(1000); // it's okay to use delay in setup()

  // ESC calibration: send max and min PWM to the ESC
  // Calibration must be done only once when you have a new ESC

  // These line must be then commented out
  // OCR2B = 255; // max PWM duty cycle value for ESC
  // OCR2A = 255; // max PWM duty cycle value for ESC
  // delay(5000);

  // always start the motor with min PWM for safety!!
  OCR2B = 125; // min PWM duty cycle value for ESC
  OCR2A = 125; // min PWM duty cycle value for ESC
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(uint8_t i = 125; i < 240; i++)
  {
    OCR2B = i;
    OCR2A = i;
    delay(100); // only for a demonstration purpose
  }
  for(uint8_t i = 240; i > 125; i--)
  {
    OCR2B = i;
    OCR2A = i;
    delay(100); // only for a demonstration purpose
  }
}
