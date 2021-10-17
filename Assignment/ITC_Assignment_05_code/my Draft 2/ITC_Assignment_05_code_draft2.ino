#define SOUTH_BUTTON_PIN 2
#define LED_S_R 4
#define LED_S_Y 5
#define LED_S_G 6

#define WEST_BUTTON_PIN 3
#define LED_W_R 7
#define LED_W_Y 8
#define LED_W_G 9

#define CROSS_BUTTON_PIN 12
#define LED_CR_R 10
#define LED_CR_G 11

#define goS     0
#define waitS   1
#define goW     2
#define waitW   3
#define Cross 4
#define dtCross 5

struct State {
    unsigned long ST_Out; // 6-bit pattern to street output
    unsigned long Time; // delay in ms units
    unsigned long Next[8];
}; // next state for inputs 0,1,2,3

typedef const struct State SType;
// static unsigned int ST_In[8];

SType FSM[6]={
    // Next[8] = 000, 001, 010, 100, 011, 110, 101, 111

    // goS
    {B10001100, 2000, {goS, waitS, waitS, goS, waitS, waitS, waitS, waitS }}, 
    // waitS
    {B10010100, 300,  {dtCross, Cross, goW, dtCross, goW, goW, Cross, goW}},
    // goW
    {B10100001, 2000, {goW, waitW, goW, waitW, waitW, waitW, waitW, waitW}},
    // waitW
    {B10100010, 300,  {dtCross, Cross, dtCross, goS, Cross, goS, Cross, Cross}},
    // Cross ***
    {B01100100, 3000, {Cross, Cross, dtCross, dtCross, dtCross, dtCross, dtCross, dtCross}}, 
    // dtCross
    {B10100100, 300,  {dtCross, dtCross, goW, goS, goW, goS, goS, goS}}

};

unsigned long S = 0; // index to the current state

void setup() {
    pinMode(LED_S_R, OUTPUT);
    pinMode(LED_S_Y, OUTPUT);
    pinMode(LED_S_G, OUTPUT);
    pinMode(SOUTH_BUTTON_PIN, INPUT);

    pinMode(LED_W_Y, OUTPUT);
    pinMode(LED_W_R, OUTPUT);
    pinMode(LED_W_G, OUTPUT);
    pinMode(WEST_BUTTON_PIN, INPUT);

    pinMode(LED_CR_R, OUTPUT);
    pinMode(LED_CR_G, OUTPUT);
    pinMode(CROSS_BUTTON_PIN, INPUT);

    Serial.begin(9600);
} 

int input, inputS, inputW, inputC;

void loop() {
    inputS = digitalRead(SOUTH_BUTTON_PIN);
    inputW = digitalRead(WEST_BUTTON_PIN);
    inputC = digitalRead(CROSS_BUTTON_PIN);

    digitalWrite(LED_S_R, FSM[S].ST_Out & B00000001);
    digitalWrite(LED_S_Y, FSM[S].ST_Out & B00000010);
    digitalWrite(LED_S_G, FSM[S].ST_Out & B00000100);

    digitalWrite(LED_W_R, FSM[S].ST_Out & B00001000);
    digitalWrite(LED_W_Y, FSM[S].ST_Out & B00010000);
    digitalWrite(LED_W_G, FSM[S].ST_Out & B00100000);
       
    digitalWrite(LED_CR_R, FSM[S].ST_Out & B10000000);
    digitalWrite(LED_CR_G, FSM[S].ST_Out & B01000000);

    delay(FSM[S].Time);

    input = inputC*3 + inputW*2 + inputS ;

    // input++;
    Serial.print("Input\t: ");
    Serial.println(input);

    Serial.print(inputS);
    Serial.print("\t");
    Serial.print(inputW);
    Serial.print("\t");
    Serial.println(inputC);

    S = FSM[S].Next[input];
}