// Slave Board 2
#include <Wire.h>
#define SLAVE_B1_B2_ADDR 1
#define MAX_TEXT_LENGHT 20

static char text[MAX_TEXT_LENGHT];
static unsigned int pos = 0;

void setup() {
	Serial.begin(4800);
  	Wire.begin(SLAVE_B1_B2_ADDR);
  	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);
}

void receiveEvent(int num){
	pos = 0;
  	while(Wire.available()){
    	text[pos] = Wire.read();
    	pos++;
  	}
 	if (text[pos - 1] == '9') {
		Serial.println(text);
		return;
	}          
  	else {text[pos - 1]++;}
  	Serial.println(text);
}

void requestEvent(){
	Wire.write(text);
}

void loop() {
}