#include <Arduino_FreeRTOS.h>
// #include "queue.h"
#include <queue.h>
#define RED 6
#define YELLOW 7
#define GREEN 8
#define SW1 10
#define SW2 11
#define SW3 12
QueueHandle_t ledQueue;

void setup()
{
  Serial.begin(9600);
  ledQueue = xQueueCreate(5,sizeof(int32_t));
  xTaskCreate(vSenderTask, "Sender Task", 128, NULL, 1,NULL);
  xTaskCreate(vReceiverTask1, "Receiver Task", 128,NULL, 1, NULL);
  xTaskCreate(vReceiverTask2, "Receiver Task", 128,NULL, 1, NULL);
  xTaskCreate(vReceiverTask3, "Receiver Task", 128,NULL, 1, NULL);
}

void vSenderTask(void *pvParameters)
{
  BaseType_t qStatus1, qStatus2, qStatus3;
  int32_t valueToSend1 = 0;
  int32_t valueToSend2 = 0;
  int32_t valueToSend3 = 0;
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  while(1)
  {
    valueToSend1= digitalRead(SW1);
    valueToSend2= digitalRead(SW2);
    valueToSend3= digitalRead(SW3);
    qStatus1= xQueueSend(ledQueue,&valueToSend1,0);
    qStatus2= xQueueSend(ledQueue,&valueToSend2,0); 
    qStatus3= xQueueSend(ledQueue,&valueToSend3,0); 
    vTaskDelay(10);
  }
}

void vReceiverTask1(void *pvParameters)
{
  unsigned long debounce1 = 0;
  int b1state = 0;
  int32_t valueReceived1;
  BaseType_t qStatus1;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(RED, OUTPUT);
  while(1)
  {
    xQueueReceive(ledQueue,&valueReceived1,xTicksToWait);
    if(debounce1 <= millis()){
      debounce1 = millis();
      digitalWrite(RED,LOW);
    }
    if (valueReceived1 == 1 && b1state == 0) {
      debounce1 += 3000;
    }
    if (debounce1 > millis()) {
      digitalWrite(RED,HIGH);
    }
    b1state = valueReceived1;
    vTaskDelay(1);
  }
}

void vReceiverTask2(void *pvParameters)
{
  unsigned long debounce2 = 0;
  int b2state = 0;
  int state = LOW;
  int32_t valueReceived2;
  BaseType_t qStatus2;
  const TickType_t xTicksToWait= pdMS_TO_TICKS(100);
  pinMode(YELLOW, OUTPUT);
  while(1)
  {
    xQueueReceive(ledQueue,&valueReceived2,xTicksToWait);
    if(valueReceived2 == 1 && b2state == 0) state = !state;
    b2state = valueReceived2;
    if(state == LOW) digitalWrite(YELLOW,LOW);
    if(state == HIGH && millis() - debounce2 >= 1000) {
      digitalWrite(YELLOW, !digitalRead(YELLOW));
      debounce2 = millis();
      }
      vTaskDelay(1);
  }
}

void vReceiverTask3(void *pvParameters)
{
  unsigned long debounce3 = 0;
  int a = 0;
  int b3state = 0;
  int state = LOW;
  int32_t valueReceived3;
  BaseType_t qStatus3;
  const TickType_t xTicksToWait= pdMS_TO_TICKS(100);
  pinMode(GREEN, OUTPUT);
  while(1)
  {
    xQueueReceive(ledQueue,&valueReceived3,xTicksToWait);
    if(state == LOW && valueReceived3 == 1 && b3state == 0){
      state = HIGH;
      debounce3 = millis();
    }
    b3state = valueReceived3;
    if(state == HIGH){
      if(millis() - debounce3 >= a && a <= 2500){
        a += 500;
        digitalWrite(GREEN, !digitalRead(GREEN));     
      }
      else if (a > 2500){
        state = LOW;
        a = 0;
      }
    }
    vTaskDelay(1);
  }
}

void loop(){}