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

int ball_posX, ball_posY;
const int ball_radius = 2;
int ball_moveX = 1, ball_moveY = 1;  
const int ball_addX = 1,  ball_addY = 1 ;

struct Bar{
    unsigned int lenght;
    unsigned int posX;
    unsigned int posY;
    unsigned int turn;
    unsigned int last_posX;
} myBar;

void drawBall();

void setup(){
    randomSeed(analogRead(A0));
    Serial.begin(115200);
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // OLED.display();

    //Set up myBar
    myBar.lenght = 25;
    myBar.posY = 28;
    myBar.last_posX = 0;

    //Set up controller
    pinMode(A2, INPUT);
    readResistor_last = 0;
    myBar.last_posX = myBar.posX;

    // Set up ball
    ball_posX = random(0 + ball_radius, SCREEN_WIDTH - ball_radius);
    ball_posY = random(0, 5);

    OLED.clearDisplay();

}

void loop() {
    readResistor_now = analogRead(A2);
    double voltageSet = 1024.0/((SCREEN_WIDTH - 1) - myBar.lenght);

    OLED.clearDisplay();
    
    // Moving by Resistor 
    Serial.println(abs(readResistor_now-readResistor_last));
    if( abs(readResistor_now - readResistor_last) > voltageSet ){
        readResistor_last = readResistor_now;
        myBar.last_posX = myBar.posX;
        myBar.posX = readResistor_now / voltageSet;
    }
    OLED.drawLine(myBar.posX, myBar.posY, (myBar.posX + myBar.lenght), myBar.posY, WHITE);

    // Ball move
    drawBall();
    ball_posX += ball_moveX;
    ball_posY += ball_moveY;
    if(ball_posX - ball_radius <= 0){
        ball_posX = ball_radius;
        ball_moveX = ball_addX ;
    }
    if(ball_posX + ball_radius >= SCREEN_WIDTH){
        ball_posX = SCREEN_WIDTH - ball_radius;
        ball_moveX = -ball_addX;
    }
    if(ball_posY - ball_radius <= 0){
        ball_moveY = ball_addY ;
    }
    if(ball_posY + ball_radius >= myBar.posY){
        // if(ball_posX >= myBar.posX && ball_posX <= myBar.posX + myBar.lenght && 
        // ball_posY + ball_radius == myBar.posY){
            ball_moveY = -ball_addY ;
        // }
        // else if (ball_posY - ball_radius >= SCREEN_HEIGHT) {
        //     OLED.clearDisplay();
        //     OLED.setCursor(5,6);
        //     OLED.setTextSize(2);
        //     OLED.setTextColor(WHITE);
        //     OLED.println("Game over!");
        //     OLED.display();
        //     delay(5000);
        //     setup();
        // }
    }
    OLED.display();
}

void drawBall(){
    OLED.drawCircle(ball_posX, ball_posY, ball_radius, WHITE);
    OLED.fillCircle(ball_posX, ball_posY, ball_radius, WHITE);
}
