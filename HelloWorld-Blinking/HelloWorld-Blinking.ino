#define LED_PIN 13
#define INPUT_PIN 12
int val;

void setup() {  
  Serial.begin(115200);
  Serial.println("Hello World!");
  DDRB = B11101111;
}

void loop() { 
  PORTB = B11111111;
  val = (PINB >> 5 & B00100000 >> 5);
  Serial.println(val);
  delay(1000);

  PORTB = B11011111;
  val = (PINB >> 5 & B00100000 >> 5);
  Serial.println(val);
  delay(1000);
}

// void setup() {  
//   Serial.begin(115200);
//   Serial.println("Hello World!");
//   *(volatile uint8_t *)0x24 = 0b11101111;
// }

// void loop() { 
//   *(volatile uint8_t *)0x25 = 0b11111111;
//   val = ((*(volatile uint8_t *)0x23) >> 5 & 0b00100000 >> 5);
//   Serial.println(val);
//   delay(1000);

//   *(volatile uint8_t *)0x25 = B11011111;
//   val = ((*(volatile uint8_t *)0x23) >> 5 & 0b00100000 >> 5);
//   Serial.println(val);
//   delay(1000);
// }

// void setup() {  
//   Serial.begin(115200);
//   Serial.println("Hello World!");
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() { 
//   digitalWrite(LED_PIN, HIGH);
//   val = digitalRead(LED_PIN);
//   Serial.println(val);
//   delay(1000);

//   digitalWrite(LED_PIN, LOW);
//   val = digitalRead(LED_PIN);
//   Serial.println(val);
//   delay(1000);
// }