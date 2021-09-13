// C++ code
//
void setup()
{
}

void loop()
{
  //LED1
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, INPUT);
  digitalWrite(7,HIGH);
  delay(250);
  //LED2
  digitalWrite(7,LOW);
  digitalWrite(6,HIGH);
  delay(250);
  //LED3
  pinMode(7,INPUT);
  pinMode(5, OUTPUT);
  delay(250);
  //LED4
  digitalWrite(6,LOW);
  digitalWrite(5,HIGH);
  delay(250);
  //LED5
  digitalWrite(5,LOW);
  pinMode(7,OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(7,HIGH);
  delay(250);
  //LED6
  digitalWrite(7,LOW); 
  digitalWrite(5,HIGH);
  delay(250);
  //LED5
  digitalWrite(5,LOW);
  digitalWrite(7,HIGH);
  delay(250);
  //LED4
  digitalWrite(7,LOW);
  pinMode(7,INPUT);
  pinMode(6, OUTPUT);
  digitalWrite(6,LOW);
  digitalWrite(5,HIGH);
  delay(250);
  //LED3
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  delay(250);
  //LED2
  pinMode(7, OUTPUT);
  pinMode(5, INPUT);
  delay(250);
  digitalWrite(6,LOW);
}