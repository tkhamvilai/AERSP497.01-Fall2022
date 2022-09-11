#include <Arduino_FreeRTOS.h>

#define LED1_PIN 5
#define LED2_PIN 6
#define LED3_PIN 7

#define LED1_INTERVAL 200
#define LED2_INTERVAL 400
#define LED3_INTERVAL 600

unsigned long LED1_previousMillis = 0;
unsigned long LED2_previousMillis = 0;
unsigned long LED3_previousMillis = 0;

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void TaskBlink3( void *pvParameters );

void setup() {
  xTaskCreate(TaskBlink1, "Blink1", 128, NULL, 0, NULL);
  xTaskCreate(TaskBlink2, "Blink2", 128, NULL, 0, NULL);
  xTaskCreate(TaskBlink3, "Blink3", 128, NULL, 0, NULL);
}

void loop() {
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  DDRD |= (1 << LED1_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount ();

  while (1) {
    int val = (PIND & (1 << LED1_PIN)) >> LED1_PIN;
    PORTD = (val) ? ( PORTD & ~(1 << LED1_PIN) ) : ( PORTD | (1 << LED1_PIN) );
    vTaskDelayUntil( &xLastWakeTime, LED1_INTERVAL / portTICK_PERIOD_MS );
  }
}

void TaskBlink2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  DDRD |= (1 << LED2_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount ();

  while (1) {
    int val = (PIND & (1 << LED2_PIN)) >> LED2_PIN;
    PORTD = (val) ? ( PORTD & ~(1 << LED2_PIN) ) : ( PORTD | (1 << LED2_PIN) );
    vTaskDelayUntil( &xLastWakeTime, LED2_INTERVAL / portTICK_PERIOD_MS );
  }
}

void TaskBlink3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  DDRD |= (1 << LED3_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount ();

  while (1) {
    int val = (PIND & (1 << LED3_PIN)) >> LED3_PIN;
    PORTD = (val) ? ( PORTD & ~(1 << LED3_PIN) ) : ( PORTD | (1 << LED3_PIN) );
    vTaskDelayUntil( &xLastWakeTime, LED3_INTERVAL / portTICK_PERIOD_MS );
  }
}