// Master Board 1
#include <Wire.h>
#define SLAVE_B1_B2_ADDR 1
#define SLAVE_B1_B3_ADDR 3

#define MAX_TEXT_LENGHT 20

static char text[MAX_TEXT_LENGHT] = "HELLO1";
static unsigned int pos = 0;
bool status = true;
int sizeText = strlen(text);

void setup() {
  	Serial.begin(4800);
  	Wire.begin();
}

void serialEvent(){
	// New text
	status = true;
	for(int n = 0; n < sizeText; n++){
		text[n] = ' ';
	}
	while (Serial.available() > 0) {		
		char inByte = Serial.read();
      	if (inByte != '\n' && (pos < MAX_TEXT_LENGHT - 2)){
        	text[pos] = inByte;
        	pos++;
      	}
    }
  	text[pos] = '1';
    text[pos + 1] = '\0';
    sizeText = strlen(text);
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
	Serial.println(text);       
  	// check();
}

void writeBus(uint8_t boardADDR){
	Serial.println(text);          
    Wire.beginTransmission(boardADDR);
	Wire.write(text);
	Wire.endTransmission(boardADDR); 
}

void check(){
  	for(int n = 0; n < sizeText; n++) {
    	if (text[n] == '9' && text[n + 1] == '\0') { 
			status = false;
		}
    }
}

void loop() {
  	delay(50);
	while (status) {
		Serial.print("Sent to B2\t\t: ");
		writeBus(SLAVE_B1_B2_ADDR);
		Serial.print("Receive from B2\t: ");
		request(SLAVE_B1_B2_ADDR); 
		Serial.print("Sent to B3\t\t: ");
		writeBus(SLAVE_B1_B3_ADDR);
		Serial.print("Receive from B3\t: ");
		request(SLAVE_B1_B3_ADDR); 
	}
}