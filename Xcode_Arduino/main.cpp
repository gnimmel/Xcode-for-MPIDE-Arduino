#include "WProgram.h"
#include "SoftwareSerial.h"
#include "LED.h"

// this is for a bug in Arduino 017/018:
extern "C" void __cxa_pure_virtual() { while (1); }


// function prototypes, compulsory


// variables
LED myLed; // = LED(); 


// setup
void setup(){
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
	
	myLed.begin(13);
	
	Serial.println("------------------");
	Serial.println("   hello world");
	Serial.println("------------------");
	
	myLed.on();
	delay(1000);
}


// loop
void loop(){
	
	myLed.blink(2, 500); // blink led 2 times, 500 ms interval
	delay(1000); // wait 1 sec
}


// do not change below
int main(void)
{
	
	init();
	
	setup();
    
	for (;;)
		loop();
	
	return 0;
}




