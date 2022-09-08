#define LED_PIN 13
#define INPUT_PIN 12
int val;

void setup() {  
  Serial.begin(115200);
  Serial.println("Hello World!");
  pinMode(LED_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);
}

void loop() { 
  digitalWrite(LED_PIN, HIGH);
  val = digitalRead(LED_PIN);
  Serial.println(val);
  delay(1000);

  digitalWrite(LED_PIN, LOW);
  val = digitalRead(LED_PIN);
  Serial.println(val);
  delay(1000);
}

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