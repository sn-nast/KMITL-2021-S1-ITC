#define button1 12
#define LED1 5
#define b2 11 //button2
#define LED2 4
#define b3 10 //button3
#define LED3 3
int Bch1 = 0;
int Bch2 = 1;
int Bch3 = 1;
unsigned long T1;
unsigned long T2;
unsigned long T3;
int c1 = 0;
int c3 = 0;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  //ReadButtonPressed
  int ReadB1 = digitalRead(button1);
  int ReadB2 = digitalRead(b2);
  int ReadB3 = digitalRead(b3);

  //CheckTimePressedButton1
  if ((millis() - T1) >= 150) {
    if (ReadB1 != Bch1 && c1 == 0) {
      T1 = millis();
    }
    else if (ReadB1 == Bch1 ) {
      c1 = 0;
    }
  }

  //CheckTimePressedButton2
  if (ReadB2 != Bch2 && digitalRead(LED1) == LOW && digitalRead(LED3) == LOW && Bch2 == 1) {
    T2 = millis();
  }

  //CheckTimePressedButton3
  if ((millis() - T3) >= 150) {
    if (ReadB3 != Bch3 && digitalRead(LED1) == LOW && c3 == 0) {
      T3 = millis();
    }
    else if (ReadB3 == Bch3 ) {
      c3 = 0;
    }
  }

  //LED1
  if (millis() - T1 <= 10 && T1 != 0 && c1 == 0) {
    digitalWrite(LED1, !digitalRead(LED1));
    c1 = 1;
  }
  if ((millis() - T1) >= 3000) {
    digitalWrite(LED1, LOW);
  }

  //LED2
  if ((millis() - T2) <= 2000 && T2 != 0 ) {
    Bch2 = ReadB2;
    if ((millis() - T2) <= 500) {
      digitalWrite(LED2, HIGH);
    }
    if ((millis() - T2) > 500 && (millis() - T2) <= 1000) {
      digitalWrite(LED2, LOW);
    }
    if ((millis() - T2) > 1000 && (millis() - T2) <= 1500) {
      digitalWrite(LED2, HIGH);
    }
    if ((millis() - T2) > 1500) {
      digitalWrite(LED2, LOW);
    }
  }
  if (ReadB2 == 1) {
    Bch2 = ReadB2;
  }

  //LED3
  if ((millis() - T3) <= 1 && T3 != 0 && c3 == 0 ) {
    digitalWrite(LED3, !digitalRead(LED3));
    c3 = 1;
  }
  if ((millis() - T3) >= 3000) {
    digitalWrite(LED3, LOW);
  }
}
