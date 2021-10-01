#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1

Adafruit_SSD1306 OLED(OLED_RESET);

void setup(){
    Serial.begin(115200);
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);

}

int i = 0;
int Ro = 1;
void loop()
{

struct Bar{
    int lenght = 25;
    int posX;
    int posY;
} myBar;

myBar.posX = 0;
myBar.posY = 31;

    OLED.clearDisplay();
    OLED.drawLine(i, myBar.posY, i + myBar.lenght, myBar.posY, WHITE);
    OLED.display();
    Serial.println(i);
    if(Ro == 1) {i++;}
    else if (Ro == 2) {i--;}

    if(Ro == 1 && i == 127 - myBar.lenght) {Ro = 2;}
    else if (Ro == 2 && i == 0) {Ro = 1;}

}
