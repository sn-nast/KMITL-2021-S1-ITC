#define G 2
#define Y 3
#define R 4
#define S1 12
#define S2 11
#define S3 10

void green(){
    digitalWrite(G, HIGH);
    delay(1000);
    digitalWrite(G, LOW);
    delay(1000);
}

void red(){
    digitalWrite(R, HIGH);
    delay(1000);
    digitalWrite(R, LOW);
    delay(1000);
}

void yellow(){
    digitalWrite(Y, HIGH);
    delay(1000);
    digitalWrite(Y, LOW);
    delay(1000);
}
void setup(){
  pinMode(R, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);

}

void loop() {
    green();
    delay(1000);
    digitalWrite(G, HIGH);
    
    yellow();
    delay(1000);
    digitalWrite(Y, HIGH);

    red();
    delay(1000);
    digitalWrite(R, HIGH);


}

