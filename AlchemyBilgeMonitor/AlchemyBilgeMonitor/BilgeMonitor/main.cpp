#include <Arduino.h>

// ATMEL ATMEGA328
//
//									                  +-\/-+
//									            PC6  1|    |28  PC5 (AI 5)
//									      (D 0) PD0  2|    |27  PC4 (AI 4)
//									      (D 1) PD1  3|    |26  PC3 (AI 3)
//				main float switch	      (D 2) PD2  4|    |25  PC2 (AI 2)
//			high water float switch	 PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//			bilge bump relay		      (D 4) PD4  6|    |23  PC0 (AI 0)
//									            VCC  7|    |22  GND
//									            GND  8|    |21  AREF
//									            PB6  9|    |20  AVCC
//									            PB7 10|    |19  PB5 (D 13)
//									 PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
//			high water alarm relay	 PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//									      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//			manual bilge switch		      (D 8) PB0 14|    |15  PB1 (D 9) PWM



#define ACTIVE		0  //FLOAT SWITCHES ARE ACTIVE LOW
#define INACTIVE	1

#define YES 1
#define NO 0

#define TIME_15_SECONDS		15000
#define TIME_30_SECONDS		30000
#define TIME_60_SECONDS		60000
#define TIME_120_SECONDS	120000
#define TIME_2_HOURS		(7200000)


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

/* read main float switch pin state*/
#define READ_MANUAL_BILGE_SWITCH() ((PINB >> PORTB0) & 0x01)


#pragma region FUNCTION_PROTOTYPES
//===============================================================================
//FUNCTION PROTOTYPES
//===============================================================================
void initializeTimers(void);


//interrupt vectors
ISR(TIMER1_COMPA_vect);
//ISR(WDT_vect);
//ISR(PCINT0_vect);
//ISR(PCINT1_vect);
//ISR(PCINT2_vect);

//===============================================================================
#pragma endregion FUNCTION_PROTOTYPES

uint8_t mainFloatSwitchState;
uint8_t highFloatSwitchState;
uint8_t manualBilgeSwitchState;
uint32_t mainFloatSwitchTrippedTime = 0;
uint32_t mainFloatSwitchClearedTime = 0;
volatile uint32_t bilgeEndTime = 0;
uint32_t currentTime = 0;
volatile uint32_t secondsCount = 0;
uint8_t bilgeCountdownSeconds = 0;
uint32_t cyclicBilgeTimerDelay = 


//===============================================================================

ISR(TIMER1_COMPA_vect){  //TIMER INTERRUPT USED FOR COUNTDOWN BEFORE TURNING OFF PUMP
	secondsCount++;
	
	if (secondsCount >= bilgeCountdownSeconds){
		secondsCount = 0;
		BILGE_OFF();
		bilgeEndTime = currentTime;
		cbi(TIMSK1, OCIE1A);		// disable timer compare interrupt
	}
	
	TCNT1 = 0;
	return;
}


int main(void)
{  //SETUP=======================================================================

	init();
	initializeTimers();
	Serial.begin(115200);
	
	//set main float switch pin to input pullup
	cbi(DDRD, PORTD2);
	sbi(PORTD,PORTD2);
	
	//set high water float switch pin to input pullup
	cbi(DDRD, PORTD3);
	sbi(PORTD,PORTD3);
	
	
	
	
	
//SETUP END=====================================================================

while(1)
{  //LOOP=======================================================================

	currentTime = millis();

//READ INPUTS
	mainFloatSwitchState = READ_MAIN_FLOAT_SWITCH();
	highFloatSwitchState = READ_HIGH_WATER_FLOAT_SWITCH();
	manualBilgeSwitchState = READ_MANUAL_BILGE_SWITCH();

//RUN BILGE MANUALLY	
	if(manualBilgeSwitchState == ACTIVE){
		delay(50);									//for debounce
		while(manualBilgeSwitchState == ACTIVE){
			BILGE_ON();
		}
		bilgeOffDelay(0);
	}
	
//RUN BILGE AUTOMATICALLY	
	if(mainFloatSwitchState == ACTIVE){
		delay(50);									//for debounce
		mainFloatSwitchTrippedTime = millis();
		while(mainFloatSwitchState == ACTIVE){
			BILGE_ON();
			mainFloatSwitchState = READ_MAIN_FLOAT_SWITCH();
			mainFloatSwitchClearedTime = millis();
		}
		bilgeOffDelay(60);
	}
	
//RUN BILGE CYCLICALLY
		
		

		
}  //LOOP END===================================================================
return(0);
}


void bilgeOffDelay(uint8_t bilgeDelaySeconds){
	bilgeCountdownSeconds = bilgeDelaySeconds;
	TCNT1 = 0;					// reset timer value to 0
	sbi(TIMSK1, OCIE1A);		// ENABLE timer compare interrupt for bilge on delay
}

void initializeTimers(void){
	/*SETUP TIMER FOR BILGE OFF DELAY*/
	cli();
	TCCR1A = 0;					// clear timer mode register 1A
	TCCR1B = 0;					// clear timer mode register 1B
	TCNT1  = 0;					// reset timer value to 0
	OCR1A = 15625;				// compare match register
	sbi(TCCR1B, WGM12);			// CTC mode
	sbi(TCCR1B, CS12);			// prescaler
	cbi(TCCR1B, CS11);			// prescaler
	sbi(TCCR1B, CS10);			// Prescaler
	
	//sbi(TIMSK1, OCIE1A);		// ENABLE timer compare interrupt
	sei();
	
		//Timer Pre-Scaler Register Settings
		/*==================================
		|	CS12	|	CS11	|	CS10	|		-Clock Select bits
		====================================
		|	0		|	0		|	1		|		-CLK / 1
		------------------------------------
		|	0		|	1		|	0		|		-CLK / 8
		------------------------------------
		|	0		|	1		|	1		|		-CLK / 64
		------------------------------------
		|	1		|	0		|	0		|		-CLK / 256
		------------------------------------
		|	1		|	0		|	1		|		-CLK /1024
		-----------------------------------*/
				

}