#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin
#define SCREEN_ADDRESS 0x3C // for Address; 0x3C for 128x32

#define X_AnalogPin A0
#define Y_AnalogPin A1
#define Z_AnalogPin A2

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int readResistor_now;
int readResistor_last;
bool statusClock = true;
unsigned int Hour = 19;
unsigned int Minute = 20;

void setup(){
    randomSeed(analogRead(A0));
    Serial.begin(9600);
    OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    OLED.display();
    OLED.clearDisplay();
}

void loop() {
    OLED.clearDisplay();
    // OLED.setTextSize(4);
    OLED.setTextColor(WHITE);
    OLED.setCursor(2,0);    

    // while (statusClock){ Clock(); }
    delay(1000);
    OLED.clearDisplay();
    OLED.setCursor(2,0);    
    OLED.setRotation(1);
    OLED.setTextSize(2);
    OLED.println(Hour);
    OLED.println(Hour);
    OLED.setTextSize(3);
    OLED.println(Hour);
    OLED.setTextSize(4);
    OLED.println(Hour);
    OLED.display();
}

void Clock(){
    OLED.clearDisplay();
    // Hour
    OLED.setCursor(2,0);
    OLED.print(Hour);
    OLED.display();

    delay(5000);
    OLED.print(":");
    // Minute
    OLED.print(Minute);
    OLED.display();
    delay(5000);
    // statusClock= false;
}

void Accelerometer {
    int xAcc = analogRead(X_AnalogPin);
    int yAcc = analogRead(Y_AnalogPin);
    int zAcc = analogRead(Z_AnalogPin);
    unsigned long timevar = millis();

    Serial.print(timevar);
    Serial.print(" ");
    Serial.print(xAcc);
    Serial.print(" ");
    Serial.print(yAcc);
    Serial.print(" ");
    Serial.println(zAcc);
}