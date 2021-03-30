
#include "PTC_timer.h"
#include "config_globale.h"

#define FREQ_CLK 22118400

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
	Time_ms++;
}
void Time_reset(void){
	Time_ms=0;
}
unsigned long int get_time_ms(void){
    return(Time_ms);
}


void Delay_1ms(void){ 
	unsigned int i;
	for(i=0; i < ((FREQ_CLK/1000)-2);i++){}
}
void Delay_1us(void){ 
	unsigned int i;
	for(i=0; i < ((FREQ_CLK/1000000)-2);i++){}
}

