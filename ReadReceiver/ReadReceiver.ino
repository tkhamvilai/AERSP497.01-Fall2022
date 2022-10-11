#define RC_CHANS 6
#define PCINT_PIN_COUNT 6
#define PCINT_RX_BITS (1<<2),(1<<4),(1<<5),(1<<6),(1<<7),(1<<0)
#define RX_PCINT_PIN_PORT PIND
#define RX_PCINT_PIN_PORT2 PINB
#define MIN_PWM_IN 1000
#define MAX_PWM_IN 2000
#define ROLL_MID_PWM 1500
#define PITCH_MID_PWM 1500
#define YAW_MID_PWM 1500

static int16_t rcData[RC_CHANS];
static uint8_t PCInt_RX_Pins[PCINT_PIN_COUNT] = {PCINT_RX_BITS};
volatile uint16_t rcValue[RC_CHANS];
volatile int16_t RC_Command[RC_CHANS];
static uint16_t edgeTime[RC_CHANS];

enum rc {
  ROLL,
  PITCH,
  THR,
  YAW,
  AUX,
  AUX2
};

void setup() {
  initPCINT();
  Serial.begin(115200);
}

void loop() {
  computeRC();
  PrintRC();
}

void PrintRC(){
  Serial.print("RC: "); 
  Serial.print(  RC_Command[ROLL]);  Serial.print(", "); 
  Serial.print(  RC_Command[PITCH]); Serial.print(", "); 
  Serial.print(  RC_Command[THR]);   Serial.print(", "); 
  Serial.print(  RC_Command[YAW]);   Serial.print(", ");
  Serial.print(  RC_Command[AUX]);   Serial.print(", "); 
  Serial.println(RC_Command[AUX2]);
}