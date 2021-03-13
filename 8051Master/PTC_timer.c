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
#include "config_globale.h"

#define FREQ_CLK 22118400

unsigned long int Time_us = 0;
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
	Time_us++;
}

unsigned long int get_time_us(void){
    return Time_us;
}

unsigned long int get_time_ms(void){
    return(Time_us/1000);
}


void Delay_1ms(void){ 
	unsigned int i;
	for(i=0; i < ((FREQ_CLK/1000)-2);i++){}
}
void Delay_1us(void){ 
	unsigned int i;
	for(i=0; i < ((FREQ_CLK/1000000)-2);i++){}
}

