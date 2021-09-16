#define B1 11
#define B2 12
#define allSegment 8
#define pressed LOW
#define correctDigit B1111110
#define greaterDigit B1011110
#define lessDigit B0001110

int numNow = 0, numNext = 1, randomNow;
int pinDigit = 2;
long timeB1 = 0, timeB2 = 0;
int outputBit[9] = {B0110000, B1101101, B1111001, B0110011, B1011011, B1011111, B1110000, B1111111, B1111011};
int checkNum[3] = {correctDigit, greaterDigit, lessDigit}, checkType;

void showLED(int toShow, int type){    // type 0: Show numbers, 1: Check
    // Blink
    for(int round = 1; round <= 5; round++){
        // Visible
        for(int c = 6; c >= 0; c--){        // count Digit in BIN
            digitalWrite(pinDigit++, !bitRead(toShow, c));
        }
        pinDigit = 2;
        if(type == 0) {return; }
        delay (100);

        // Invisible
        if(type == 1) {
            for(int c = 6; c >= 0; c--){    // count Digit in BIN
                digitalWrite(pinDigit++, HIGH);
            }
            delay (100);
            if(toShow == checkNum[0] && round == 5) {return; }
        }
        pinDigit = 2;
    }
    showLED(outputBit[numNow - 1], 0);
}

void count_number_button(){
    timeB1 = millis();
    numNow = numNext;
    showLED(outputBit[numNow - 1], 0);      // Show number on LED
    if(numNow == 9) {numNext = 1;}
    else            {numNext++;}
}

void guess_number_button(){    
    timeB2 = millis();
    if(numNow == randomNow)  {
        checkType = 0;
        while(numNow == randomNow) {
            randomNow = random(1, 10);
            Serial.print("Random number (new): ");
            Serial.println(randomNow);
        }
    }
    else if (numNow > randomNow)  {checkType = 1;}
    else if (numNow < randomNow)  {checkType = 2;}
    showLED(checkNum[checkType], 1);        // Show 0, G, L on LED
    pinDigit = 2;
}

void setup() {
    Serial.begin(19200);
    randomSeed(analogRead(A0));
    randomNow = random(1, 10);
    Serial.print("Random number : ");
    Serial.println(randomNow);
    for(int c = 2; c <= 8; c++){   // a-g leg = pin 2-8
        pinMode(c, OUTPUT);
        digitalWrite(c, HIGH);
    }
    pinMode(B1, INPUT_PULLUP);
    pinMode(B2, INPUT_PULLUP);
}

void loop() {
    int readB1 = digitalRead(B1);
    int readB2 = digitalRead(B2);

    /* -------- PART 1 : Count numbers ----------*/
    if(readB1 == pressed && millis() - timeB1 >= 200){ count_number_button();}
    /* ------- PART 2 : Guess the numbers --------*/    
    if(readB2 == pressed && millis() - timeB2 >= 200){ guess_number_button();}
}