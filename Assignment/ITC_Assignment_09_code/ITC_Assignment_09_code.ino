#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <math.h>
#include <TimerOne.h>

// Adafruit screen
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   32
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Alert LED
#define LED_PIN 7

// Button
#define PRESSED LOW
const int B1_PIN = 4;
const int B2_PIN = 5;
const int B3_PIN = 6;
const int BT1 = 0;
const int BT2 = 1;
const int BT3 = 2;

struct _Button {
    int Read;
    int LastRead;
    const int Pin;
    bool NowPressed;
    unsigned long PressedTime;
    unsigned long ReleasedTime;
    unsigned long CountPressedTime;
};

_Button Button[3] = {
    {0, 0, B1_PIN, false, 0, 0, 0},
    {0, 0, B2_PIN, false, 0, 0, 0},
    {0, 0, B3_PIN, false, 0, 0, 0}
};

_Button *p1 = &Button[BT1];
_Button *p2 = &Button[BT2];
_Button *p3 = &Button[BT3];

const int LONG_PRESS_TIME  = 2000;
const int SHORT_PRESS_TIME  = 1000;
const int DEBOUCE_TIME = 300;
const int SHORT_PRESSED_TYPE = 1, LONG_PRESSED_TYPE = 2;

// Accelerometer [ADXL335]
const unsigned int X_AXIS_PIN = 0;
const unsigned int Y_AXIS_PIN = 1;
const unsigned int Z_AXIS_PIN = 2;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = { X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN };
const unsigned int BUFFER_SIZE = 50;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };
    // ACC Caribrate
    const int X_RATIO = 69, X_ORIGIN = 351;
    const int Y_RATIO = 69, Y_ORIGIN = 347;
    const int Z_RATIO = 70, Z_ORIGIN = 349;

const unsigned int CLOCK_MODE = 0, STOPWATCH_MODE = 1, TIMER_MODE = 2, ALARM_MODE= 3;
unsigned int NowSelectedMode = CLOCK_MODE;
unsigned int digitPos = 0;
struct _ClockSetupMode {
    byte Digit[4];
    String name;
};
_ClockSetupMode ClockSetupMode[3] = {
    {0, 0, 0, 0, "Clock"},
    {0, 0, 0, 0, "Stopwatch"},
    {0, 0, 0, 0, "Timer"}
};
bool openSelectMode = false;
bool openSetup = false;

struct _ClockMode {
    byte Hour = 0;
    byte Minute = 0;
    byte Second = 0;
    byte LastSecond = 0;
    bool StartState = false;  
    unsigned long LastTimeChangeStart;
    bool Alert = false;
};
_ClockMode ClockMode[3];
_ClockMode *cm_Main = &ClockMode[CLOCK_MODE];

// EEPROM
int eeAddress_Hour = 0, eeAddress_Minute = 1;  

void setup(){
    Serial.begin(9600);

    OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    OLED.clearDisplay();
    
    // Read time
    cm_Main->Hour = EEPROM.read(eeAddress_Hour);
    cm_Main->Minute = EEPROM.read(eeAddress_Minute);
    Timer1.initialize(1000000);                    // Interrupt every 1 sec (1.0E+6 microsecs)
    Timer1.attachInterrupt(CountTime);

    NowSelectedMode = CLOCK_MODE;

    pinMode(B1_PIN, INPUT_PULLUP);
    pinMode(B2_PIN, INPUT_PULLUP);
    pinMode(B3_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}

void loop() {
    Serial.print(get_x());
    Serial.print(" ");
    Serial.print(get_y());
    Serial.print(" ");
    Serial.println(get_z());
    if (openSelectMode == false){
        if (openSetup == false){
            if (NowSelectedMode  == CLOCK_MODE)             { Clock();}
            else if (NowSelectedMode  == STOPWATCH_MODE)    { Stopwatch();}
            else if (NowSelectedMode == TIMER_MODE)         { Timer();}
            CheckSetup();
        }
        else {SetupMode();}
    }
    else if (openSelectMode == true) {SelectMode();}
    KeepInEEPROM();
}

void Clock(){
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    short Horizontal = 0, Vertical = 1;
    short clockType;

    // LED Alert
    cm_Main->Alert = true;
    LedAlert(cm_Main, 2);

    // Set rotation
    if (get_y() < DegreeRatio(-20, 'y') && get_z() > DegreeRatio(-45, 'z')) {
        OLED.setRotation(1);
        clockType = Vertical;
    }
    else if (get_y() > DegreeRatio(45, 'y') && get_z() > DegreeRatio(-10, 'z')) {
        OLED.setRotation(3);
        clockType = Vertical;
    }
    else if (get_x() < DegreeRatio(-30, 'x') && get_z() > DegreeRatio (-45, 'z')){
        OLED.setRotation(2);
        clockType = Horizontal;
    }
    else {
        OLED.setRotation(0);
        clockType = Horizontal;
    }
    
    // Display
    if (clockType == Horizontal) {
        OLED.setTextSize(4);
        OLED.setCursor(6,2);
        // Hour
        if (cm_Main->Hour < 10) { OLED.print(0); }
        OLED.print(cm_Main->Hour);
        OLED.print(":");
        // Minute
        if (cm_Main->Minute < 10) { OLED.print(0); }
        OLED.print(cm_Main->Minute);
        OLED.display();
    }
    else if (clockType == Vertical){
        // display Hour
        OLED.setTextSize(1);
        OLED.setCursor(3, 3);
        OLED.println("Hour");
        OLED.println(" ");
        OLED.setTextSize(2);
        OLED.setCursor(3, 17);
        if (cm_Main->Hour < 10) { OLED.print(0); }
        OLED.println(cm_Main->Hour);
        OLED.println(" ");

        // display Min
        OLED.setTextSize(1);
        OLED.setCursor(6, 45);
        OLED.println("Min");
        OLED.println(" ");
        OLED.setTextSize(2);
        OLED.setCursor(3, 59);
        if (cm_Main->Minute < 10) { OLED.print(0); }
        OLED.println(cm_Main->Minute);
        OLED.println(" ");

        // display Sec
        OLED.setTextSize(1);
        OLED.setCursor(6, 87);
        OLED.println("Sec");
        OLED.println(" ");
        OLED.setTextSize(2);
        OLED.setCursor(3, 101);
        if (cm_Main->Second < 10) { OLED.print(0); }
        OLED.println(cm_Main->Second);
        OLED.display();
    }
}

// ACC specify position
int get_axis(const int axis) {
    delay(1);
    buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
    buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
    long sum = 0;
    for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
        sum += buffer[axis][i];
    }
    return round(sum / BUFFER_SIZE);
}
int get_x() { return get_axis(0); }
int get_y() { return get_axis(1); }
int get_z() { return get_axis(2); }

// Convert Degree to ACC number
int DegreeRatio(int Angle, char Axis){
    // Degree to Radian
    double Radian = (Angle*PI) / 180;
    int Ratio, Origin;
    switch (Axis) {
    case 'x': {
        Ratio =  X_RATIO;
        Origin = X_ORIGIN;
        break;
    }
    case 'y': {
        Ratio =  Y_RATIO;
        Origin = Y_ORIGIN;
        break;
    }    
    case 'z': {
        Ratio =  Z_RATIO;
        Origin = Z_ORIGIN;
        break;
    }
    default:
        break;
    }
    int ConvertToACC = Origin + (sin(Radian) * Ratio);
    return ConvertToACC;
}

// ISR
void CountTime(void){
    cm_Main->LastSecond = cm_Main->Second;
    if (cm_Main->Second == 59){
        cm_Main->Second = 0;
        cm_Main->Minute++;
        if (cm_Main->Minute == 60) {
            cm_Main->Hour++;
            cm_Main->Minute = 0;
            if (cm_Main->Hour == 24){
                cm_Main->Hour = 0;
            }
        }
        return;
    }
    cm_Main->Second++; 
}

// Keep time
void KeepInEEPROM(void){
    EEPROM.update(eeAddress_Hour, cm_Main->Hour);
    EEPROM.update(eeAddress_Minute, cm_Main->Minute);
}

void Stopwatch(void){
    _ClockMode *cm = &ClockMode[STOPWATCH_MODE];
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);

    // display Minute
    OLED.setTextSize(4);
    OLED.setCursor(6,2);
    if (cm->Minute < 10) { OLED.print(0); }
    OLED.print(cm->Minute);
    OLED.print(":");
    // display Second
    if (cm->Second < 10) { OLED.print(0); }
    OLED.print(cm->Second);
    OLED.display();

    // Check pressed
    CheckPressedInNotClockMode(cm);

    // LED Alert
    if (cm->StartState == true) {LedAlert(cm, 3);}
    else   {LedAlert(cm, 2);}

    // Stopwatch part
    if (cm->StartState == true && millis() - cm->LastTimeChangeStart > DEBOUCE_TIME) {
        cm->Alert = true;
        if (cm->LastSecond != cm_Main->LastSecond){
            cm->LastSecond = cm_Main->LastSecond;
            if (cm->Second == 59) {
                cm->Second  = 0;
                cm->Minute++;
                if (cm->Minute == 60){
                    cm->Minute = 0;
                    cm->StartState = false;
                }
                return;
            }
            cm->Second++; 
        }
    }
}

void Timer(void){
    _ClockMode *cm = &ClockMode[TIMER_MODE];
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);

    // display Minute
    OLED.setTextSize(4);
    OLED.setCursor(6,2);
    if (cm->Minute < 10) { OLED.print(0); }
    OLED.print(cm->Minute);
    OLED.print(":");
    // display Second
    if (cm->Second < 10) { OLED.print(0); }
    OLED.print(cm->Second);
    OLED.display();

    // Check pressed
    CheckPressedInNotClockMode(cm);

    // LED Alert
    LedAlert(cm, 1);

    // Timer part
    if (cm->StartState == true && millis() - cm->LastTimeChangeStart > DEBOUCE_TIME) {
        if (cm->LastSecond != cm_Main->LastSecond){
            cm->LastSecond = cm_Main->LastSecond;
            if (cm->Second == 0){
                if (cm->Minute != 0) {
                    cm->Second = 59;
                    cm->Minute--;
                }
                else {
                    cm->StartState = false;
                    cm->Alert = true;
                }
                return;
            }
            cm->Second--; 
        }
    }
}

void SelectMode(){
    // Select mode interface
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setRotation(0);
    OLED.setTextSize(1);
    OLED.setCursor(0, 0);
    OLED.println("Press B1 to select.");
    OLED.setTextSize(2);
    OLED.setCursor(0, (SCREEN_HEIGHT / 2 )- 1);
    OLED.print(">");
    OLED.setCursor(20 , (SCREEN_HEIGHT / 2 )- 1);
    OLED.print(ClockSetupMode[NowSelectedMode].name);
    int sizeMode = sizeof(ClockSetupMode)/sizeof(ClockSetupMode[0]);
    OLED.display();

    // Select Mode
    if (CheckShortPressedFromLongPressed(p1) == true) {
        openSelectMode = false;
        openSetup = false;
    }
    // Scroll down
    if (CheckShortPressed(p2) == true) {
        if(NowSelectedMode < sizeMode - 1) { NowSelectedMode++;}
        else {NowSelectedMode = 0;}
    }
    // Scroll up
    else if (CheckShortPressed(p3) == true) {
        if(NowSelectedMode > 0) { NowSelectedMode--;}
        else {NowSelectedMode = sizeMode - 1;}
    }
}

void CheckSetup(){
    // long press B1 -> Select Mode
    if (CheckLongPressed(p1) == true) {openSelectMode = true; }
    // long press B2 -> Edit Time
    else if (CheckLongPressed(p2) == true && NowSelectedMode != STOPWATCH_MODE) {
        openSetup = true;
    }
}

void SetupMode() {
    _ClockSetupMode *cs = &ClockSetupMode[NowSelectedMode];
    _ClockMode *cm = &ClockMode[NowSelectedMode];
    cm->StartState = false;

    // LED alert
    bool tempAlert = cm->Alert;
    cm->Alert = true;
    LedAlert(cm, 1);
    cm->Alert = tempAlert;

    // Keep data
    int size = sizeof(cs->Digit)/sizeof(cs->Digit[0]);
    if (NowSelectedMode == CLOCK_MODE) {
        cs->Digit[1] = cm_Main->Hour % 10;
        cs->Digit[0] = (cm_Main->Hour - cs->Digit[1])/10;
        cs->Digit[3] = cm_Main->Minute % 10;
        cs->Digit[2] = (cm_Main->Minute - cs->Digit[3])/10;
    }
    else {
        cs->Digit[1] = cm->Minute % 10;
        cs->Digit[0] = (cm->Minute - cs->Digit[1])/10;
        cs->Digit[3] = cm->Second % 10;
        cs->Digit[2] = (cm->Second - cs->Digit[3])/10;
    }

    // Close setup
    if (CheckShortPressed(p1) == true) {
        openSetup = false;
        p2->NowPressed = false;
    }
    // Change digit
    if (CheckShortPressedFromLongPressed(p2) == true) {
        if (cs->Digit[digitPos] < 9) {cs->Digit[digitPos]++;}
        else {cs->Digit[digitPos] = 0;}
    }
    // Select digit
    if (CheckShortPressed(p3) == true) {
        if (digitPos < size - 1) {digitPos++;}
        else if(digitPos == size- 1) {digitPos = 0;}
    }

    // Alert display in setup mode
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setRotation(0);
    OLED.setCursor(6,2);
    OLED.setTextSize(4);
    if(cm_Main->Second % 2 == 0) {OLED.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);}
    for (int n = 0; n < size; n++){
        if (n == digitPos && cm_Main->Second % 2 == 0 && p2->Read != PRESSED)  {OLED.setTextColor(BLACK);}
        else {OLED.setTextColor(WHITE);}
        OLED.print(cs->Digit[n]);
        if (n == 1) {
            OLED.setTextColor(WHITE);
            OLED.print(":");
        }
    }
    OLED.display();

    // Setup num
    if (NowSelectedMode == CLOCK_MODE) {
        cm_Main->Hour = cs->Digit[0]*10 + cs->Digit[1];
        cm_Main->Minute = cs->Digit[2]*10 + cs->Digit[3];
        if (openSetup == false) {
            if (cm_Main->Hour > 23) {cm_Main->Hour = 0;}
            if (cm_Main->Minute > 59) {cm_Main->Minute = 0;}
        }
    }
    else {
        cm->Minute = cs->Digit[0]*10 + cs->Digit[1];
        cm->Second = cs->Digit[2]*10 + cs->Digit[3];
        if (openSetup == false) {
            if (cm->Second > 59) {cm->Second = 0;}
        }
    }
}

bool CheckLongPressed(_Button *p){
    p->Read = digitalRead(p->Pin);
    bool LongPressed = false;
    if (p->Read == PRESSED &&  p->LastRead != PRESSED){
        p->PressedTime = millis();
        p->NowPressed = true;
        p->CountPressedTime++;
    }
    else if (p->Read != PRESSED && p->LastRead == PRESSED ){
        p->ReleasedTime = millis();
        p->NowPressed = false;
        p->CountPressedTime = 0;
    }
    if (p->NowPressed == true){ 
        p->CountPressedTime = millis() - p->PressedTime;   
    }
    if (p->CountPressedTime > LONG_PRESS_TIME) { 
        LongPressed = true;
        p->CountPressedTime = 0;
    }
    p->LastRead = p->Read;
    return LongPressed;
}

bool CheckShortPressed(_Button *p){
    p->Read = digitalRead(p->Pin);
    bool ShortPressed = false;
    if (p->NowPressed == true) {return ShortPressed;}
    if (p->Read == PRESSED && millis() - p->PressedTime > DEBOUCE_TIME){
        p->PressedTime = millis();
        ShortPressed = true;
    }
    p->CountPressedTime = 0;
    return ShortPressed;
}

bool CheckShortPressedFromLongPressed(_Button *p){
    p->Read = digitalRead(p->Pin);
    bool ShortPressed = false;
    if (p->Read == PRESSED && p->LastRead != PRESSED && p->CountPressedTime < LONG_PRESS_TIME){
        p->PressedTime = millis();
        ShortPressed = true;
    }
    else if (p->Read != PRESSED && p->LastRead == PRESSED ){p->CountPressedTime = 0;}
    p->LastRead = p->Read;
    return ShortPressed;
}

void CheckPressedInNotClockMode (_ClockMode *cm) {
    // Button 2 - start stop
    if (CheckShortPressed(p2) == true){
        //  Can't press in Timer mode if Timer not yet set
        if (NowSelectedMode != TIMER_MODE ||
            (NowSelectedMode == TIMER_MODE  && (cm->Minute > 0 || cm->Second > 0))){
            cm->StartState = !cm->StartState;
        }
    }
    // Button 3 - Clear
    else if (CheckShortPressed(p3) == true && cm->StartState == false){
        cm->Second = 0;
        cm->Minute = 0;
        cm->Hour = 0;
        cm->Alert = false;
    }
}

void LedAlert(_ClockMode *cm, int type){
    int Frequency, Range;
    if (type == 1) {
        Frequency = 1;
        Range = 2;
    }
    else if (type == 2){
        Frequency = 0;
        Range = 1;
    }
    else if (type == 3) {
        Frequency = 1;
        Range = 3;
    }
    if (cm->Alert == true && cm_Main->Second % Range == Frequency) {
        digitalWrite(LED_PIN, HIGH);
    }
    else {digitalWrite(LED_PIN, LOW);}
}   