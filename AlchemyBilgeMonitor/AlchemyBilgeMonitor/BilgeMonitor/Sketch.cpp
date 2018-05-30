/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

#define BILGE_RELAY_12V 4
#define BILGE_RELAY_GND 5
#define ON 1
#define OFF 0

#define BILGE_ON() digitalWrite(BILGE_RELAY_12V, ON); //digitalWrite(BILGE_RELAY_GND, ON);
#define BILGE_OFF() digitalWrite(BILGE_RELAY_12V, OFF);   //digitalWrite(BILGE_RELAY_GND, OFF);

//===============================================================================
//			MACROS
//===============================================================================
#ifndef cbi
/* clear a bit in a register*/
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
/* set a bit in a register*/
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//===============================================================================


void setup() {
  // put your setup code here, to run once:
  //pinMode(BILGE_RELAY_GND, OUTPUT);
  pinMode(BILGE_RELAY_12V, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
	BILGE_ON();
	delay(120000);
	BILGE_OFF();
	delay(7200000);
}
