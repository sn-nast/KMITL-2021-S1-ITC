#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#define RED 6
#define YELLOW 7
#define GREEN 8
#define SW1 10
#define SW2 11
#define SW3 12
SemaphoreHandle_t xMutex2;
SemaphoreHandle_t xMutex;
void setup()
{
  xMutex = xSemaphoreCreateMutex();
  xMutex2 = xSemaphoreCreateMutex();
  xTaskCreate(LEDred, "RED", 100, NULL, 1, NULL);
  xTaskCreate(LEDyellow, "YELLOW", 100, NULL, 1, NULL);
  xTaskCreate(LEDgreen, "RED", 100, NULL, 1, NULL);
}

void LEDred(void *pvParameters) {
  pinMode(SW1, INPUT);
  pinMode(RED, OUTPUT);
  unsigned long a = 0;
  int lastsw1 = LOW;
  bool state = false;
  while (1) {
    int Readsw1 = digitalRead(SW1);
    if (Readsw1 == HIGH && lastsw1 == LOW) {
      a = millis();
      state = !state;
    }
    if (state == true) {
      xSemaphoreTake( xMutex, ( TickType_t ) 0) ;
      if (millis() - a >= 50 && a!= 0) {
        if (millis() - a <= 3050) {
          digitalWrite(RED, HIGH);
        }
        else {
          digitalWrite(RED, LOW);
          xSemaphoreGive( xMutex );
          state = false;
        }
      }
    } 
    else {
      digitalWrite(RED, LOW);
      xSemaphoreGive( xMutex );
    }
    lastsw1 = Readsw1;
    vTaskDelay(1);
  }
}

void LEDyellow(void *pvParameters) {
  pinMode(SW2, INPUT);
  pinMode(YELLOW, OUTPUT);
  unsigned long a = 0;
  unsigned long b = 0;
  int lastsw2 = LOW;
  bool state = false;
  while(1) {
    if (xSemaphoreTake(xMutex,  ( TickType_t ) 5  ) == pdFALSE) {
      xSemaphoreGive( xMutex );
      state = false;
      digitalWrite(YELLOW, LOW);
    } 
    else if(xSemaphoreTake(xMutex2, ( TickType_t ) 5 ) == pdFALSE){
      xSemaphoreGive( xMutex );
      state = false;
      digitalWrite(YELLOW, LOW);
    }
    int Readsw2 = digitalRead(SW2);
    if (Readsw2 == HIGH && lastsw2 == LOW ) {
      a = millis();
      b = 50;
      state = true;
    }
    if (state == true) {
      if (millis() - a >= 50 && a!= 0) {
        if ((millis() - a >= b) && (b <= 1550)) {
          digitalWrite(YELLOW, !digitalRead(YELLOW));
          b += 500;
        }
      }
    } else digitalWrite(YELLOW, LOW);
    lastsw2 = Readsw2;
    vTaskDelay(1);
  }
}

void LEDgreen(void *pvParameters) {
  pinMode(SW3, INPUT);
  pinMode(GREEN, OUTPUT);
  unsigned long a = 0;
  int lastsw3 = LOW;
  bool state = false;
  while (1) {
    int Readsw3 = digitalRead(SW3);
    if (Readsw3 == HIGH && lastsw3 == LOW) {
      if (xSemaphoreTake(xMutex, 0) == pdTRUE) {
        xSemaphoreGive(xMutex);
        a = millis();
        state = !state;
      }
    }
    if (state == true) {
      xSemaphoreTake(xMutex2, 0);
      if (millis() - a >= 50 && a != 0) {
        if (millis() - a <= 3050){
          digitalWrite(GREEN, HIGH);
        }
        else {
          digitalWrite(GREEN, LOW);
          xSemaphoreGive( xMutex2 );
          state = false;
        }
      }
    } else {
      digitalWrite(GREEN, LOW);
      xSemaphoreGive( xMutex2 );
    }
    lastsw3 = Readsw3;
    vTaskDelay(1);
  }
}
void loop() {}