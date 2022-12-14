#define LED_PIN 13
#define INPUT_PIN 12
int val;

void setup() {  
  Serial.begin(115200);
  Serial.println("Hello World!");
  DDRB |= (1 << PB5);
  DDRB &= ~(1 << PB4);
}

void loop() { 
  PORTB |= (1 << PB5);
  val = (PINB & (1 << PB4)) >> 4;
  Serial.println(val);
  delay(1000);

  PORTB &= ~(1 << PB5);
  val = (PINB & (1 << PB4)) >> 4;
  Serial.println(val);
  delay(1000);
}

// void setup() {  
//   Serial.begin(115200);
//   Serial.println("Hello World!");
//   DDRB = B11101111;
// }

// void loop() { 
//   PORTB = B11111111;
//   val = (PINB >> 5 & B00100000 >> 5);
//   Serial.println(val);
//   delay(1000);

//   PORTB = B11011111;
//   val = (PINB >> 5 & B00100000 >> 5);
//   Serial.println(val);
//   delay(1000);
// }

// void setup() {  
//   Serial.begin(115200);
//   Serial.println("Hello World!");
//   *(volatile uint8_t *)0x24 = 0b11101111;
// }

// void loop() { 
//   *(volatile uint8_t *)0x25 = 0b11111111;
//   val = ((*(volatile uint8_t *)0x23) >> 4 & 0b00010000 >> 4);
//   Serial.println(val);
//   delay(1000);

//   *(volatile uint8_t *)0x25 = 0b11011111;
//   val = ((*(volatile uint8_t *)0x23) >> 4 & 0b00010000 >> 4);
//   Serial.println(val);
//   delay(1000);
// }

// void setup() {  
//   Serial.begin(115200);
//   Serial.println("Hello World!");
//   pinMode(LED_PIN, OUTPUT);
//   pinMode(INPUT_PIN, INPUT_PULLUP);
// }

// void loop() { 
//   digitalWrite(LED_PIN, HIGH);
//   val = digitalRead(INPUT_PIN);
//   Serial.println(val);
//   delay(1000);

//   digitalWrite(LED_PIN, LOW);
//   val = digitalRead(INPUT_PIN);
//   Serial.println(val);
//   delay(1000);
// }