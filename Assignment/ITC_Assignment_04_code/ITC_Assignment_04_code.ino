#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1

Adafruit_SSD1306 OLED(OLED_RESET);

struct Bar{
    int lenght = 25;
    int posX;
    int posY;
    int turn;
} myBar;

void setup(){
    Serial.begin(115200);
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    myBar.posX = 0;
    myBar.posY = 31;
    myBar.turn = 1;

}

void loop() {
// Auto move
    // OLED.clearDisplay();
    // OLED.drawLine(myBar.posX, myBar.posY, myBar.posX + myBar.lenght, myBar.posY, WHITE);
    // OLED.display();
    // Serial.println(myBar.posX);
    // if(myBar.turn == 1) {myBar.posX++;}
    // else if (myBar.turn  == 2) {myBar.posX--;}
    // if(myBar.turn  == 1 && myBar.posX == 127 - myBar.lenght) {myBar.turn  = 2;}
    // else if (myBar.turn  == 2 && myBar.posX == 0) {myBar.turn  = 1;}

}
