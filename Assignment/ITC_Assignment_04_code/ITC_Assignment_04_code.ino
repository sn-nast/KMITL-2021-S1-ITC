#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int readResistor_now;
int readResistor_last;

struct Bar{
    int lenght;
    int posX, posY, turn;
} myBar;

struct Ball{
    int posX, posY;
    int radius;
    int moveX, moveY;  
    int addX, addY;
    int timeNewBall = 100; 
} ball;

void drawBall();

void setup(){
    randomSeed(analogRead(A0));
    Serial.begin(115200);
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    //Set up myBar
    myBar.lenght = 25;
    myBar.posY = 28;
    myBar.turn = 1 ;

    //Set up controller
    pinMode(A2, INPUT);
    readResistor_last = 0;

    // Set up ball
    ball.posX = random(0 + ball.radius, SCREEN_WIDTH - ball.radius);
    ball.posY = random(0, ball.radius );
    ball.radius = 2;
    ball.addX = 1;
    ball.addY = 1;
    ball.moveX = ball.addX;
    ball.moveX = ball.addY;

    OLED.clearDisplay();
}

void loop() {
    readResistor_now = analogRead(A2);
    double voltageSet = 1024.0/((SCREEN_WIDTH - 1) - myBar.lenght);

    OLED.clearDisplay();
    
    // Moving by Resistor 
    if( abs(readResistor_now - readResistor_last) > voltageSet ){
        readResistor_last = readResistor_now;
        myBar.posX = readResistor_now / voltageSet;
    }
    OLED.drawLine(myBar.posX, myBar.posY, (myBar.posX + myBar.lenght), myBar.posY, WHITE);

    // Ball move
    drawBall();
    if(myBar.turn == 1) {
        delay(1);
        ball.timeNewBall--;
        if (ball.timeNewBall == 0) {
            myBar.turn = 0;
            ball.timeNewBall = 100;
        }
    }
    else {
        ball.posX += ball.moveX;
        ball.posY += ball.moveY;
    }

    // Direction
    if(ball.posX - ball.radius <= 0){
        ball.posX = ball.radius;
        ball.moveX = ball.addX ;
    }
    if(ball.posX + ball.radius >= SCREEN_WIDTH){
        ball.posX = SCREEN_WIDTH - ball.radius;
        ball.moveX = -ball.addX;
    }
    if(ball.posY - ball.radius <= 0){
        ball.moveY = ball.addY ;
    }
    if(ball.posY + ball.radius >= myBar.posY){
        if(ball.posX >= myBar.posX && ball.posX <= myBar.posX + myBar.lenght && 
        ball.posY + ball.radius == myBar.posY){
            ball.moveY = -ball.addY ;
        }
        else if (ball.posY - ball.radius >= SCREEN_HEIGHT) {
            OLED.clearDisplay();
            OLED.setCursor(5,6);
            OLED.setTextSize(2);
            OLED.setTextColor(WHITE);
            OLED.println("Game over!");
            OLED.display();
            delay(3000);
            setup();
        }
    }
    OLED.display();
}

void drawBall(){
    OLED.drawCircle(ball.posX, ball.posY, ball.radius, WHITE);
    OLED.fillCircle(ball.posX, ball.posY, ball.radius, WHITE);
}
