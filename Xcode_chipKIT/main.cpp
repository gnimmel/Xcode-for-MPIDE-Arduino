#include "WProgram.h"
#include <p32xxx.h>

// prototypes
void blink(int n, int d);

// variables
void setup() {                
	pinMode(13, OUTPUT);     
	pinMode(43, OUTPUT);     
}


void loop(){
	
	blink(3, 333); // blink led 2 times, 500 ms interval
	delay(1000); // wait 1 sec
}




void blink(int n, int d){
	for (int i=0;i<n;i++)
	{
		digitalWrite(13, HIGH);   // set the LED on
		delay(d/2);
		digitalWrite(43, LOW);    // set the LED off
		delay(d/2);
		digitalWrite(13, LOW);    // set the LED off
		delay(d/2);
		digitalWrite(43, HIGH);   // set the LED on
		delay(d/2);
	}
}


int main(void)
{
	init();
	
	setup();
    
	for (;;)
		loop();
	
	return 0;
}

