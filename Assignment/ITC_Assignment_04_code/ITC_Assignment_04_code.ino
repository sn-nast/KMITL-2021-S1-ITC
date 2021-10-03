#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#define OLED_RESET -1
const short max_weight = 127, max_height = 31;
int readResistor_now;
int readResistor_last;
unsigned long time_now;
unsigned long time_last; 

Adafruit_SSD1306 OLED(OLED_RESET);

struct Bar{
    unsigned int lenght;
    unsigned int posX;
    unsigned int posY;
    unsigned int turn;
    unsigned int last_posX;
} myBar;

void setup(){
    randomSeed(analogRead(A0));
    Serial.begin(9600);
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.display();

    //Set up myBar
    myBar.lenght = 25;
    myBar.posY = 31;
    // myBar.turn = 1;
    myBar.last_posX = 0;
    //Set up controller
    pinMode(A2, INPUT);
    readResistor_last = 0;
    myBar.last_posX = myBar.posX;
}

void loop() {
    time_now = millis();
    readResistor_now = analogRead(A2);
    const long double voltageSet = 1024/((max_weight - 1) - myBar.lenght);

    OLED.clearDisplay();
    // Moving.
    int long rateWhat = 33;
    Serial.println(time_now);
    Serial.println(time_last);
    Serial.println("**************");
    
    if(time_now - time_last > 50){
        time_last = time_now;
        // time_last += rateWhat;
        // readResistor_last = readResistor_now;
        Serial.print("now\t\t: ");
        Serial.println(readResistor_now);
        Serial.print("last\t: ");
        Serial.println(readResistor_last);
        Serial.print("diff\t: ");
        Serial.println((readResistor_now - readResistor_last));
        // Serial.println("-------");
        
        if( myBar.posX == myBar.last_posX && abs(readResistor_now-readResistor_last) > voltageSet ){
            readResistor_last = readResistor_now;
            myBar.last_posX = myBar.posX;
            myBar.posX = readResistor_now / voltageSet;
            OLED.drawLine(myBar.posX, myBar.posY, (myBar.posX + myBar.lenght), myBar.posY, WHITE);
        }
    }
    OLED.display();
}

