#define G 2
#define Y 3
#define R 4
#define S1 12
#define S2 11
#define S3 10
#define pressPU LOW
#define pressPD HIGH

void green(){
    digitalWrite(G, HIGH);
    delay(3*1000);
    digitalWrite(G, LOW);
    delay(3*1000);

}

void red(){
    digitalWrite(R, HIGH);
    delay(3*1000);
    digitalWrite(R, LOW);
}

void yellow(){
    digitalWrite(Y, HIGH);
    delay(500);
    digitalWrite(Y, LOW);
    delay(500);
    digitalWrite(Y, HIGH);
    delay(500);
    digitalWrite(Y, LOW);
    delay(500);
}
void greenOFF(){
    digitalWrite(S3, LOW);
} 
void yellowOFF(){
    digitalWrite(S2, LOW);
} 

int s_now[3], s_last[3] = {LOW, LOW, LOW};
long dbc_lasy[3] = {0, 0, 0}, dbc[3] = {150, 150, 150} ;
int reading[3] ;

void Debounce(int SW){
    int c;
    if      (SW == S1) { c = 0;}
    else if (SW == S2) { c = 1;}
    else if (SW == S3) { c = 2;}

    if((reading[c]) != (s_last[c])) {(s_last[c]) = millis();   } // millis เก็บค่าเวลา ณ ตอนนั้น
    if(millis() - (s_last[c]) > (dbc[c])) {
        if(reading[c] != s_now[c]){
            s_now[c] = reading[c];
        }
    }
    s_last[c] = reading[c];
}

void setup(){
    Serial.begin(19200);
    pinMode(R, OUTPUT);
    pinMode(Y, OUTPUT);
    pinMode(G, OUTPUT);

    pinMode(A1, OUTPUT);
    digitalWrite(A1, LOW);

    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    pinMode(S3, INPUT_PULLUP);

    // attachInterrupt(digitalPinToInterrupt(R), redOFF, CHANGE);
    attachInterrupt(digitalPinToInterrupt(G), greenOFF, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Y), yellowOFF, CHANGE);
}

void loop() {

    reading[0] = digitalRead(S1);   // read_S1
    reading[1] = digitalRead(S2);   // read_S2
    reading[2] = digitalRead(S3);   // read_S3
    Debounce(S1);
    Debounce(S2);
    Debounce(S3);
    bool read_S1 = reading[0];
    bool read_S2 = reading[1]; 
    bool read_S3 = reading[2]; 
    delay(100);

    if (read_S1 == pressPD) {
        Serial.println("S1 press");
        if(digitalRead(R) == LOW) { digitalWrite(R, HIGH); }
        else digitalWrite(R, LOW);
    }
    if (read_S3 == pressPU) {
        Serial.println("S3 press");
        if(digitalRead(G) == LOW) { digitalWrite(G, HIGH);}
        else digitalWrite(G, LOW);
        read_S3 = HIGH;
    }
    if (read_S2 == pressPU) {
        Serial.println("S2 press");
        if(digitalRead(Y) == LOW) { digitalWrite(Y, HIGH);}
        else digitalWrite(Y, LOW);
    }
    // // if(read_S1 != pressPD && read_S3 != pressPU && read_S2 == pressPU)
    // // { yellow();}
    // Serial.println("-------------");
    
}