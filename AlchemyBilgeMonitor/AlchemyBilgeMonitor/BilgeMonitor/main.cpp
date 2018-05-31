#include <Arduino.h>

// ATMEL ATMEGA328
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM



#define ACTIVE		0  //FLOAT SWITCHES ARE ACTIVE LOW
#define INACTIVE	1

#define YES 1
#define NO 0

#define TIME_15_SECONDS		15000
#define TIME_30_SECONDS		30000
#define TIME_60_SECONDS		60000
#define TIME_120_SECONDS	120000


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

#define BILGE_ON()					\
	do {							\
		sbi(DDRD, PORTD4);			\
		sbi(PORTD,PORTD4);			\
	} while(0)


#define BILGE_OFF()					\
	do {							\
		sbi(DDRD, PORTD4);			\
		cbi(PORTD,PORTD4);			\
	} while(0)
	
#define HIGH_WATER_ALARM_ON()		\
	do {							\
		sbi(DDRD, PORTD6);			\
		sbi(PORTD,PORTD6);			\
	} while(0)


#define HIGH_WATER_ALARM_OFF()		\
	do {							\
		sbi(DDRD, PORTD6);			\
		cbi(PORTD,PORTD6);			\
	} while(0)
	
/* read main float switch pin state*/
#define READ_MAIN_FLOAT_SWITCH() ((PIND >> PORTD2) & 0x01)

/* read high water float switch pin state*/
#define READ_HIGH_WATER_FLOAT_SWITCH() ((PIND >> PORTD3) & 0x01)

//===============================================================================

uint8_t mainFloatSwitchState;
uint8_t highFloatSwitchState;
uint8_t bilgeActive = 0;
uint32_t mainFloatSwitchTrippedTime = 0;
uint32_t bilgeStartTime = 0;
uint32_t mainFloatSwitchClearedTime = 0;
uint32_t bilgeActivePumpDelayTime = TIME_60_SECONDS;
uint32_t bilgeEndTime = 0;
uint32_t currentTime = 0;


int main(void)
{  //SETUP=======================================================================

	init();
	Serial.begin(115200);
	
	//main float switch to input pullup
	cbi(DDRD, PORTD2);
	sbi(PORTD,PORTD2);
	
	//high water float switch to input pullup
	cbi(DDRD, PORTD3);
	sbi(PORTD,PORTD3);
	
	
	
	
	
//SETUP END=====================================================================

while(1)
{  //LOOP=======================================================================

	mainFloatSwitchState = READ_MAIN_FLOAT_SWITCH();
	if(mainFloatSwitchState == ACTIVE){
		mainFloatSwitchTrippedTime = millis();
		bilgeActive = YES;
		}
		
	while(mainFloatSwitchState == 0){
		BILGE_ON(); 
		mainFloatSwitchState = READ_MAIN_FLOAT_SWITCH();
		mainFloatSwitchClearedTime = millis();
	}
	currentTime = millis;
	if((currentTime - mainFloatSwitchClearedTime) > bilgeActivePumpDelayTime){
		BILGE_OFF();
		bilgeEndTime = millis();
	}






		
}  //LOOP END===================================================================
return(0);
}

void beginBilgePumpCountdown(void){
	
}

//void setup() {
//
//
//}
//
//void loop() {
  //// put your main code here, to run repeatedly:
	//BILGE_ON();
	//delay(120000);
	//BILGE_OFF();
	//delay(7200000);
//}
