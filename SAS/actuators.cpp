#include "actuators.h"
#include "Arduino.h"

Actuators::Actuators()
{

}

Actuators::~Actuators()
{

}

void Actuators::init()
{
    // set PORTD (D3) as an OUTPUT
  DDRD |= (1 << DDD3);
  // set PORTB (D9,D10,D11) as an OUTPUT
  DDRB |= ((1 << DDB1)|(1 << DDB2)|(1 << DDB3));

  // set Compare Output Mode for OC2B
  TCCR2A |= (1 << COM2B1); TCCR2A &= ~(1 << COM2B0);

  // set Compare Output Mode for OC2A
  TCCR2A |= (1 << COM2A1); TCCR2A &= ~(1 << COM2A0);

  // set PWM generation mode to Phase Correct
  TCCR2B &= ~(1 << WGM22); TCCR2A &= ~(1 << WGM21); TCCR2A |= (1 << WGM20);
  
  // set prescaling factor to 64
  TCCR2B |= (1 << CS22); TCCR2B &= ~(1 << CS21); TCCR2B &= ~(1 << CS20);

  TCCR1A |= (_BV(COM1A1) | _BV(COM1B1)); //set CompareOutputMode to D9, D10

  // wait to make sure everything is set
  delay(1000); // it's okay to use delay in setup()
}

void Actuators::calibrate()
{
  // ESC calibration: send max and min PWM to the ESC
  // Calibration must be done only once when you have a new ESC

  OCR2B = MAX_DUTY_CYCLE; // max PWM duty cycle value for ESC
  OCR2A = MAX_DUTY_CYCLE; // max PWM duty cycle value for ESC
  OCR1B = MAX_DUTY_CYCLE; // max PWM duty cycle value for ESC
  OCR1A = MAX_DUTY_CYCLE; // max PWM duty cycle value for ESC
  delay(5000);
  OCR2B = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  OCR2A = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  OCR1B = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  OCR1A = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  delay(5000);
}

void Actuators::stop()
{
  OCR2B = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  OCR2A = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  OCR1B = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
  OCR1A = MIN_DUTY_CYCLE; // min PWM duty cycle value for ESC
}

void Actuators::update(uint16_t pwm[MOTOR_NUM])
{
  for(uint8_t i=0; i<MOTOR_NUM; i++) pwm[i] = constrain(pwm[i], MIN_PWM_OUT, MAX_PWM_OUT);
  
  OCR2B = pwm[FRONT_RIGHT] >> 3; //D3  CCW
  OCR1A = pwm[FRONT_LEFT]  >> 3; //D9  CW
  OCR1B = pwm[REAR_LEFT]   >> 3; //D10 CCW
  OCR2A = pwm[REAR_RIGHT]  >> 3; //D11 CW
}

void Actuators::print()
{
  Serial.print("Actuators: ");
  Serial.print(  OCR2B);   Serial.print(", "); 
  Serial.print(  OCR1A);  Serial.print(", "); 
  Serial.print(  OCR1B); Serial.print(", ");  
  Serial.println(OCR2A);
}
