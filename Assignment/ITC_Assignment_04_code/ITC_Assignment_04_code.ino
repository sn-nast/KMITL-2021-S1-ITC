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

int ball_posX, ball_posY;
const int ball_radius = 3;
int ball_moveX = 1, ball_moveY = 1; 


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
    Serial.begin(115200);
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.display();

    //Set up myBar
    myBar.lenght = 25;
    myBar.posY = 31;
    myBar.last_posX = 0;

    //Set up controller
    pinMode(A2, INPUT);
    readResistor_last = 0;
    myBar.last_posX = myBar.posX;
}

void loop() {
    readResistor_now = analogRead(A2);
    double voltageSet = 1024.0/((max_weight - 1) - myBar.lenght);

    OLED.clearDisplay();
    
    // Moving by Resistor 
        Serial.println(voltageSet);
        if( abs(readResistor_now-readResistor_last) > voltageSet ){
            readResistor_last = readResistor_now;
            myBar.last_posX = myBar.posX;
            myBar.posX = readResistor_now / voltageSet;
        }
            OLED.drawLine(myBar.posX, myBar.posY, (myBar.posX + myBar.lenght), myBar.posY, WHITE);
    // Ball move
    drawBall();
    ball_posX += ball_moveX;
    ball_posY += ball_moveY;
    if(ball_posX - ball_radius == 0){
        ball_moveX = 1;
    }
    if(ball_posX + ball_radius == max_weight){
        ball_moveX = -1;
    }
    if(ball_posY - ball_radius == 0){
        ball_moveY = 1;
    }
    if(ball_posY + ball_radius == max_height){
        if(ball_posX > myBar.posX && ball_posX <= myBar.posX + myBar.lenght){
            ball_moveY = -1;
        }
        else {
            OLED.clearDisplay();
            OLED.setCursor(5,6);
            OLED.setTextSize(2);
            OLED.setTextColor(WHITE);
            OLED.println("Game over!");
            OLED.display();
            delay(5000);
            setup();
        }
    }
    OLED.display();
}

void drawBall(){
    OLED.drawCircle(ball_posX, ball_posY, ball_radius, WHITE);
    OLED.fillCircle(ball_posX, ball_posY, ball_radius, WHITE);
    OLED.display();
}

