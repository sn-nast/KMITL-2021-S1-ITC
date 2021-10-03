#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#define OLED_RESET -1
const short max_weight = 127, max_height = 31;
const int SPEAKER_PIN = 3; 

int readResistor_now;
int readResistor_last;
unsigned long time_now;
unsigned long time_last;

int ball_posX, ball_posY;
const int ball_radius = 3;
int ball_moveX = 1, ball_moveY = 1; 

int numTones = 10;
int tones[] = {261,277,294,311,330,349,370,392,415,440};
int soundType;
Adafruit_SSD1306 OLED(OLED_RESET);

struct Bar{
    unsigned int lenght;
    unsigned int posX;
    unsigned int posY;
    unsigned int turn;
    unsigned int last_posX;
} myBar;

void drawBall();
void checkSound();

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

    // Set up ball
    ball_posX = random(0 + ball_radius, max_weight - ball_radius);
    ball_posY = random(0 + ball_radius, (1/3)*max_height);

    // Set up speaker
    pinMode(SPEAKER_PIN, OUTPUT);
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
    int timeS = 15;
    int tone1 = 250;
    drawBall();
    ball_posX += ball_moveX;
    ball_posY += ball_moveY;
    if(ball_posX - ball_radius == 0){
        ball_moveX = 1;
        soundType = 1;
    }
    if(ball_posX + ball_radius == max_weight){
        ball_moveX = -1;
        soundType = 1;
    }
    if(ball_posY - ball_radius == 0){
        ball_moveY = 1;
        soundType = 1;
    }
    if(ball_posY + ball_radius == max_height){
        if(ball_posX > myBar.posX && ball_posX <= myBar.posX + myBar.lenght){
            ball_moveY = -1;
            soundType = 1;
        }
        else {
            OLED.clearDisplay();
            OLED.setCursor(5,6);
            OLED.setTextSize(2);
            OLED.setTextColor(WHITE);
            OLED.println("Game over!");
            OLED.display();
            soundType = 2;
            delay(5000);
            setup();
        }
    }
    OLED.display();
    checkSound();
}

void drawBall(){
    OLED.drawCircle(ball_posX, ball_posY, ball_radius, WHITE);
    OLED.fillCircle(ball_posX, ball_posY, ball_radius, WHITE);
    OLED.display();
}

void checkSound(){
    if(soundType == 1) {
        for(int i = 4000; i <5000; i++){
            tone(SPEAKER_PIN, i, 2);
        }
        soundType = 0;
    }
    if(soundType == 2) {
        tone(SPEAKER_PIN, 3951, 20);
        soundType = 0;
    }
    noTone(SPEAKER_PIN);
}
