//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                         Interrupt General Function                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#define RX_PIN_CHECK(chan)                             \
  if (mask & PCInt_RX_Pins[chan]) {                    \
    if (!(pin & PCInt_RX_Pins[chan])) {                \
      dTime = cTime - edgeTime[chan];                  \
      if (MIN_PWM_IN < dTime && dTime < MAX_PWM_IN) {  \
        rcValue[chan] = dTime;                         \
      }                                                \
    }                                                  \
    else edgeTime[chan] = cTime;                       \
  }                                                  
  
void RX_PIN_MASK(){
  cli(); //disable other interrupts at this point
  uint8_t mask;
  uint8_t pin;
  uint16_t cTime, dTime;
  static uint8_t PCintLast;  
  
  pin = RX_PCINT_PIN_PORT; //read PIN
  mask = pin ^ PCintLast; //indicate which bit change 
  cTime = micros(); //keep only 16 bits
  sei(); //re-enable other interrupts at this point, the rest of this interrupt is not so time critical and can be interrupted safely
  PCintLast = pin; //memorize the state of all PIN7:0
   
  //write PPM to each PIN
  RX_PIN_CHECK(ROLL);
  RX_PIN_CHECK(PITCH);
  RX_PIN_CHECK(THR);
  RX_PIN_CHECK(YAW);
  RX_PIN_CHECK(AUX);
}

void RX_PIN_MASK2(){
  cli(); //disable other interrupts at this point
  uint8_t mask;
  uint8_t pin;
  uint16_t cTime, dTime;
  static uint8_t PCintLast;  
  
  pin = RX_PCINT_PIN_PORT2; //read PIN
  mask = pin ^ PCintLast; //indicate which bit change 
  cTime = micros(); //keep only 16 bits
  sei(); //re-enable other interrupts at this point, the rest of this interrupt is not so time critical and can be interrupted safely
  PCintLast = pin; //memorize the state of all PIN7:0

  RX_PIN_CHECK(AUX2);
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                       Interrupt Filtering Function                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

uint16_t readRawRC(uint8_t chan) {
  uint16_t data;
  uint8_t oldSREG;
  
  oldSREG = SREG; 
  cli(); // Let's disable interrupts
  
  data = rcValue[chan]; // Let's copy the data Automically

  SREG = oldSREG; 
  sei();// Let's enable the interrupts
  return data; // We return the value correctly copied when the IRQ's where disabled
}

void computeRC() {
  static int16_t rcData4Values[RC_CHANS][4], rcDataMean[RC_CHANS];
  static uint8_t rc4ValuesIndex = 0;
  uint8_t chan,a;
  
  rc4ValuesIndex++;
  
  for(chan = 0; chan < RC_CHANS; chan++) {
    rcData4Values[chan][rc4ValuesIndex%4] = readRawRC(chan);
    rcDataMean[chan] = 0;
    for(a = 0; a < 4; a++) rcDataMean[chan] += rcData4Values[chan][a];
    rcDataMean[chan] = (rcDataMean[chan]+2)>>2;
    if(rcDataMean[chan] < rcData[chan]-3) rcData[chan] = rcDataMean[chan]+2;
    if(rcDataMean[chan] > rcData[chan]+3) rcData[chan] = rcDataMean[chan]-2;
  }
  
  mapRCcmd();
}

void mapRCcmd(){
  for(uint8_t chan = 0; chan < RC_CHANS; chan++) {
    rcData[chan] = constrain(rcData[chan], MIN_PWM_IN, MAX_PWM_IN);
  }
  
  RC_Command[ROLL]   = rcData[ROLL]  - ROLL_MID_PWM;
  RC_Command[PITCH]  = rcData[PITCH] - PITCH_MID_PWM;
  RC_Command[THR]    = rcData[THR];
  RC_Command[YAW]    = rcData[YAW]   - YAW_MID_PWM;
  RC_Command[AUX]    = rcData[AUX];
  RC_Command[AUX2]   = rcData[AUX2];
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//            PIN mapping : CH1   CH2   CH3   CH4   CH5   CH6            //
//                          D2    D4    D5    D6    D7    D8             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
void initPCINT(){  
  DDRD &= ~(_BV(DDD2) | _BV(DDD4) | _BV(DDD5) | _BV(DDD6) | _BV(DDD7)); //set PORTD PIN 2,4,5,6,7 as INPUTS
  PORTD |= (_BV(PORTD2) | _BV(PORTD4) | _BV(PORTD5) | _BV(PORTD6) | _BV(PORTD7)); //pull-up HIGH

  DDRB &= ~(_BV(DDB0)); //set PORTB PIN 0 as an INPUT
  PORTB |= (_BV(PORTB0)); //pull-up HIGH

  cli();
  PCICR  |= (_BV(PCIE2) | _BV(PCIE0)); // turn on PCMSK2 and PCMSK0
  PCMSK2 |= (_BV(PCINT18) | _BV(PCINT20) | _BV(PCINT21) | _BV(PCINT22) | _BV(PCINT23)); // enable Pin Change Interrupt for PORTD PIN 2,4,5,6,7
  PCMSK0 |= (_BV(PCINT0)); // enable Pin Change Interrupt for PORTB PIN 0
  sei();
}

ISR (PCINT2_vect){
  RX_PIN_MASK();
}

ISR (PCINT0_vect){
  RX_PIN_MASK2();
}