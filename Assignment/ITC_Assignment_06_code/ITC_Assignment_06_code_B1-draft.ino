// Master Board 1
#include <Wire.h>
#define SLAVE_B1_B2_ADDR 1
#define SLAVE_B1_B3_ADDR 3

#define MAX_TEXT_LENGHT 20

static char text[MAX_TEXT_LENGHT] = "HELLO1";
static unsigned int pos = 0;
bool status = true;
int sizeText = strlen(text);
int BOARD_NAME = 1;

void setup() {
  	Serial.begin(4800);
  	Wire.begin();
	Serial.println(text);
}

void serialEvent(){
	// Start typing
	while (Serial.available() > 0) {		
		char inByte = Serial.read();
      	if (inByte != '\n' && (pos < MAX_TEXT_LENGHT - 2)){
        	text[pos] = inByte;
        	pos++;
      	}
    }
  	text[pos] = BOARD_NAME;
  	Serial.println(text);
}   

void request(uint8_t boardADDR){
	pos = 0;
	Wire.requestFrom(boardADDR, sizeText);
  	while (Wire.available()) {		
		char inByte = (char) Wire.read();
    	if (inByte != '\0' && (pos < sizeText)){
    		text[pos] = inByte;
    		pos++;
    	}
    }
   	if (text[pos - 1] == '9') {check();}
	Serial.println(text);          
}

void writeBus(uint8_t boardARRD){
    Wire.beginTransmission(boardARRD);
	Wire.write(text);
	Wire.endTransmission(boardARRD); 
}

void check(){
  	for(int n = 0; n < sizeText; n++) {
    	if (text[n] == '9' && text[n + 1] == '\0') { 
			Serial.println(text);
			status = false;
          	text[n] = '1';
		}
    }
}

void loop() {
  	delay(50);
	while (status) {
		writeBus(SLAVE_B1_B2_ADDR);
		request(SLAVE_B1_B2_ADDR); 
		writeBus(SLAVE_B1_B3_ADDR);
		request(SLAVE_B1_B3_ADDR); 
	}
}