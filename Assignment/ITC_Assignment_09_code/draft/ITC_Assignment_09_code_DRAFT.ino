#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#define RED_PIN    4
#define YELLOW_PIN 3
#define GREEN_PIN  2
#define SW1_PIN    12
#define SW2_PIN    11
#define SW3_PIN    10
#define SW1_PRESSED HIGH
#define SW2_PRESSED LOW
#define SW3_PRESSED LOW

SemaphoreHandle_t xSemaphoreLed[2];
SemaphoreHandle_t xMutex;
int stateRED = !SW1_PRESSED;
unsigned long time = 0;
void setup() {
    Serial.begin(9600);
    // pinMode(RED_PIN, OUTPUT);
    // pinMode(GREEN_PIN, OUTPUT);
    // pinMode(YELLOW_PIN, OUTPUT);
    // pinMode(SW1_PIN, INPUT);
    // pinMode(SW2_PIN, INPUT_PULLUP);
    // pinMode(SW3_PIN, INPUT);
    xSemaphoreLed[0] = xSemaphoreCreateBinary();
    // xBinarySemaphore = xSemaphoreCreateBinary();
    // xMutex = xSemaphoreCreateMutex(); 
    // xTaskCreate(LedOnTask, "LedON",100,NULL,1,NULL);
    // xTaskCreate(LedoffTask, "LedOFF", 100,NULL,1,NULL);

    xTaskCreate(LedRedTask, "RED", 100, NULL, 1, NULL);
    // xTaskCreate(LedYellowTask, "YELLOW", 100, NULL, 1, NULL);
    // xTaskCreate(LedGreenTask, "GREEN", 100, NULL, 1, NULL);
    // xSemaphoreGive(xBinarySemaphore);
    xSemaphoreGive(xSemaphoreLed[0]);
    
}

void loop() {
    // Serial.println(" Now in loop");
            // digitalWrite(RED_PIN, SW1_PRESSED);

}

// void LedOnTask(void *pvParameters) {
//     while(1) {
//         xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
//         Serial.println("Inside LedOnTask");
//         // digitalWrite(LED,LOW);
//         xSemaphoreGive(xBinarySemaphore);
//         vTaskDelay(1);
//     }
// }
// void LedoffTask(void *pvParameters) {
//     while(1) {
//         xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
//         Serial.println("Inside LedffTask");
//         // digitalWrite(LED,HIGH);
//         xSemaphoreGive(xBinarySemaphore);
//         vTaskDelay(1);
//     }
// }

void LedRedTask(void *pTask){
    pinMode(SW1_PIN, INPUT);
    pinMode(RED_PIN, OUTPUT);
    unsigned long pressTime = 0;

    int SW1_read = digitalRead(SW1_PIN);
    // int stateLed = !SW1_PRESSED;

    Serial.println("RED TASK");

    while(1){
        // xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        
        Serial.println("----------- Red task ------------");
        if (SW1_read == SW1_PRESSED &&  millis() - time > 100){
            time = millis();
            if ()
        }
        if(xSemaphoreTake(xSemaphoreLed[0], portMAX_DELAY) == pdTRUE){
            time += 
            Serial.println("L1 Taken");
            digitalWrite(RED_PIN, SW1_PRESSED);
            // vTaskDelay(300);
            Serial.println("pass 3s");
            digitalWrite(RED_PIN, !SW1_PRESSED);
            // xSemaphoreGive(xSemaphoreLed[0]);
        }
        vTaskDelay(1);
    }
}
void LedYellowTask(void *pTask){}
void LedGreenTask(void *pTask){}