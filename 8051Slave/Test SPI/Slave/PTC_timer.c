//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_timer.c
// Destination: 8150F020 Carte Master
// Description: Fonctions du temps
//------------------------------------------------------

#include "PTC_timer.h"
#include "c8051F020.h"
#include "c8051F020_SFR16.h"

#define FREQ_CLK 22118400
#define DELAY_1US 1

unsigned long int Time_ms = 0;
//fonctions de temp
//necesite une variable globale Time_us
//necesite l'activation des timer comme tels:
//     - Timer 0 : * Mode 2: 8-bit Counter/Timer with Auto-Reload
//                 * SYSCLK/12


//Time_us retourne le temp en us depuis le demarage du micoC
//Time_us retourne le temp en ms depuis le demarage du micoC

//Delay_1ms fait une pause de 1ms
//Delay_1us fait une pause de 1us


void Time_increment(void){
	Time_ms+=10;
}


unsigned long int get_time_ms(void){
    return(Time_ms);
}

void Delay(const unsigned long time_ms){
	unsigned long i;
	for (i=0; i < time_ms; i++){
		Delay_1ms();
	}
}

void Delay_1ms(void){ 
	unsigned int i;
	for(i=0; i < ((FREQ_CLK/65999)-2);i++){}
}
void Delay_1us(void){ 
	unsigned int i;
	i++;
}
void Delay_10us(void){
	unsigned int i;
	// for(i=0; i <20;i++){} Florin's version
	for(i=0; i <10;i++){}
}

void Interrupt_Time(void) interrupt 16 {//interruption declancher par l'overflow du Timer 3 (toutes les ms)
	T4CON &= ~(1<<7); //interrupt flag
    Time_increment();
}