#define LED_W_R 4
#define LED_W_Y 5
#define LED_W_G 6
#define WEST_BUTTON_PIN 2

#define LED_S_R 8
#define LED_S_Y 9
#define LED_S_G 10
#define SOUTH_BUTTON_PIN 3

#define LED_WALK_R 13
#define LED_WALK_G 12
#define WALK_BUTTON_PIN 11

#define goW               0
#define waitW_FROM_S      1
#define waitW_FROM_WALK   2
#define goS               3
#define waitS_FROM_WALK   4
#define waitS_FROM_W      5
#define goWalk            6
#define waitWalk_FROM_W   7
#define waitWalk_FROM_S   8
#define waitWalk_FROM_W2  9
#define waitWalk_FROM_S2  10
#define waitWalk_FROM_W3  11
#define waitWalk_FROM_S3  12
#define waitWalk_FROM_W4  13
#define waitWalk_FROM_S4  14
#define waitWalk_FROM_W5  15
#define waitWalk_FROM_S5  16
#define waitWalk_FROM_W6  17
#define waitWalk_FROM_S6  18

struct State {
  unsigned long ST_Out; 
  unsigned long Time; 
  unsigned long Next[8];
};
typedef const struct State SType;
SType FSM[19] = {
  {B01001100, 1000, {goW, goW, waitW_FROM_S, waitW_FROM_S, waitW_FROM_WALK, waitW_FROM_WALK, waitW_FROM_S, waitW_FROM_S}},
  {B01001010, 300, {goS, goS, goS, goS, goWalk, goWalk, goS, goS}},
  {B01001010, 300, {goWalk, goWalk, goS, goS, goWalk, goWalk, goS, goS}},
  {B01100001, 1000, {goS, waitS_FROM_W, goS, waitS_FROM_W, waitS_FROM_WALK, waitS_FROM_WALK, waitS_FROM_WALK, waitS_FROM_WALK}},
  {B01010001, 300, {goWalk, goW, goWalk, goW, goWalk, goWalk, goWalk, goWalk}},
  {B01010001, 300, {goW, goW, goW, goW, goWalk, goWalk, goWalk, goWalk}},
  {B10001001, 1000, {goWalk, waitWalk_FROM_W, waitWalk_FROM_S, waitWalk_FROM_W, goWalk, waitWalk_FROM_W, waitWalk_FROM_S, waitWalk_FROM_W}},
  {B00001001, 50, {waitWalk_FROM_W2, waitWalk_FROM_W2, waitWalk_FROM_W2, waitWalk_FROM_W2, waitWalk_FROM_W2, waitWalk_FROM_W2, waitWalk_FROM_W2, waitWalk_FROM_W2}},
  {B00001001, 50, {waitWalk_FROM_S2, waitWalk_FROM_S2, waitWalk_FROM_S2, waitWalk_FROM_S2, waitWalk_FROM_S2, waitWalk_FROM_S2, waitWalk_FROM_S2, waitWalk_FROM_S2}},
  {B10001001, 50, {waitWalk_FROM_W3, waitWalk_FROM_W3, waitWalk_FROM_W3, waitWalk_FROM_W3, waitWalk_FROM_W3, waitWalk_FROM_W3, waitWalk_FROM_W3, waitWalk_FROM_W3}},
  {B10001001, 50, {waitWalk_FROM_S3, waitWalk_FROM_S3, waitWalk_FROM_S3, waitWalk_FROM_S3, waitWalk_FROM_S3, waitWalk_FROM_S3, waitWalk_FROM_S3, waitWalk_FROM_S3}},
  {B00001001, 50, {waitWalk_FROM_W4, waitWalk_FROM_W4, waitWalk_FROM_W4, waitWalk_FROM_W4, waitWalk_FROM_W4, waitWalk_FROM_W4, waitWalk_FROM_W4, waitWalk_FROM_W4}},
  {B00001001, 50, {waitWalk_FROM_S4, waitWalk_FROM_S4, waitWalk_FROM_S4, waitWalk_FROM_S4, waitWalk_FROM_S4, waitWalk_FROM_S4, waitWalk_FROM_S4, waitWalk_FROM_S4}},
  {B10001001, 50, {waitWalk_FROM_W5, waitWalk_FROM_W5, waitWalk_FROM_W5, waitWalk_FROM_W5, waitWalk_FROM_W5, waitWalk_FROM_W5, waitWalk_FROM_W5, waitWalk_FROM_W5}},
  {B10001001, 50, {waitWalk_FROM_S5, waitWalk_FROM_S5, waitWalk_FROM_S5, waitWalk_FROM_S5, waitWalk_FROM_S5, waitWalk_FROM_S5, waitWalk_FROM_S5, waitWalk_FROM_S5}},
  {B00001001, 50, {waitWalk_FROM_W6, waitWalk_FROM_W6, waitWalk_FROM_W6, waitWalk_FROM_W6, waitWalk_FROM_W6, waitWalk_FROM_W6, waitWalk_FROM_W6, waitWalk_FROM_W6}},
  {B00001001, 50, {waitWalk_FROM_S6, waitWalk_FROM_S6, waitWalk_FROM_S6, waitWalk_FROM_S6, waitWalk_FROM_S6, waitWalk_FROM_S6, waitWalk_FROM_S6, waitWalk_FROM_S6}},
  {B10001001, 50, {goW, goW, goS, goW, goW, goW, goS, goW}},
  {B10001001, 50, {goS, goW, goS, goW, goS, goW, goS, goW}},
};
unsigned long S = 0; // index to the current state
void setup() {
  Serial.begin(9600);
  pinMode(LED_W_R, OUTPUT);
  pinMode(LED_W_Y, OUTPUT);
  pinMode(LED_W_G, OUTPUT);
  pinMode(WEST_BUTTON_PIN, INPUT);

  pinMode(LED_S_R, OUTPUT);
  pinMode(LED_S_Y, OUTPUT);
  pinMode(LED_S_G, OUTPUT);
  pinMode(SOUTH_BUTTON_PIN, INPUT);
  
  pinMode(LED_WALK_R, OUTPUT);
  pinMode(LED_WALK_G, OUTPUT);
  pinMode(WALK_BUTTON_PIN, INPUT);
}
int input, input1, input2, input3;
void loop() {
  digitalWrite(LED_W_R, FSM[S].ST_Out & B00000001);
  digitalWrite(LED_W_Y, FSM[S].ST_Out & B00000010);
  digitalWrite(LED_W_G, FSM[S].ST_Out & B00000100);

  digitalWrite(LED_S_R, FSM[S].ST_Out & B00001000);
  digitalWrite(LED_S_Y, FSM[S].ST_Out & B00010000);
  digitalWrite(LED_S_G, FSM[S].ST_Out & B00100000);

  digitalWrite(LED_WALK_R, FSM[S].ST_Out & B01000000);
  digitalWrite(LED_WALK_G, FSM[S].ST_Out & B10000000);

  delay(FSM[S].Time);
  input1 = digitalRead(WEST_BUTTON_PIN);
  input2 = digitalRead(SOUTH_BUTTON_PIN);
  input3 = digitalRead(WALK_BUTTON_PIN);
  input = input3 * 4 + input2 * 2 + input1;

  Serial.print("input\t= ");
  Serial.println(input);
  Serial.print("input1\t= ");
  Serial.println(input1);
  Serial.print("input2\t= ");
  Serial.println(input2);
  Serial.print("input3\t= ");
  Serial.println(input3);

  S = FSM[S].Next[input];

  Serial.print("S = ");
  Serial.println(S);
}