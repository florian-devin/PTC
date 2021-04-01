//-----------------------------------------------------------------------------
// TP4_Main.c
//-----------------------------------------------------------------------------
// AUTH: 
// DATE: 
//
// Target: C8051F02x
// Tool chain: KEIL Microvision 4
//
//-----------------------------------------------------------------------------
#include <C8051F020.h>
#include "c8051F020_SFR16.h"
//#include "TP4_Lib_Config_Globale_8051F020.h"
#include "TP4_Lib_Divers.h"
#include "PTC_timer.h"
#include "config_globale.h"

// Prototypes de Fonctions 

void Lumiere_Stop (void);
void Lumiere (unsigned char Intensite,unsigned char Lum_ON, unsigned char Lum_OFF, unsigned char Lum_Nbre) ;
void interrupt_mli(void);
void interrupt_time(void);

//variables globales

sbit LED=P3^3;
int cpt=0;
int intensite=0;
unsigned int reg;
int go=0;
sbit TEST=P3^4;

//

/*
1 dixieme de seconde, impulsions 1 centieme de seconde
 du dixieme de microseconde qui vont aller de 0 a 100 
 faire attention au modulo  (demander a maxime)
 changer ddixieme de micor en centieme de seconde 
 
*/


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
 
	  Init_Device();  // Appel des configurations globales
	  
// D�but Insertion Code Configuration des p�riph�riques ***********************
		EA=1;
		IE|=0x20;
	/*
	  CKCON&=0x08;
		TL0=0x48;
		TCON|=0x10; //allumer timer 0

*/TEST=0;
	P3MDOUT|=0x08;
	Lumiere(70,10,10,3);
	
	
	
	WDTCN = 0xde;                              // Disable watchdog timer
	WDTCN = 0xad;
	XBR0 = 0x08;                               // enable CEX0 at P0.0
	XBR2 = 0x40;                               // enable crossbar and weak // pull-ups
	P0MDOUT = 0x01;                             // set P0.0 output state to // push-pull
	P1MDOUT = 0x20;                             // set P1.6 output to // push-pull (LED)// configure the 
	
	
// Fin Code Initialisations ***************************************************
	
	
// D�but Insertion Code Phase D�marrage ***************************************	
    
// Fin Code phase D�marrage	***************************************************
	
	
	while(1)
    {
    }				               	
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Fonctions de configuration des divers p�riph�riques
//-----------------------------------------------------------------------------
// Ins�rez vos fonctions de configuration ici
//Allume le pointeur lunineux en clignotant
//@param Intensite intensite de la lumiere (signal PWM)
//@param Lum_ON Temps durant lequelle la lumiere reste alume
//@param Lum_OFF Temps durant la led est eteinte
//@param Lum_Nbre Nomble de cycle d'allumage 
void Lumiere (unsigned char Intensite,unsigned char Lum_ON, unsigned char Lum_OFF, unsigned char Lum_Nbre);

void Lumiere (unsigned char Intensite,unsigned char Lum_ON, unsigned char Lum_OFF, unsigned char Lum_Nbre) {
	unsigned int i;
	int cond;
	intensite=Intensite;
	for(i=0; i<Lum_Nbre;i++){
		Time_reset();
		cond=1;
		go=1;
		while(cond){
			if(get_time_ms()>=Lum_ON*100){
				go=0;
			}
			if(get_time_ms()>=(Lum_OFF+Lum_ON)*100){
				cond=0;
			}
		}
	}
}

void Lumiere_Stop (void){
	LED=0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Fonctions d'interruptions
/*
void interrupt_time(void)  1{
	T2CON&=0x80;
	Time_increment(); //incr�mentation toutes les dms
}
*/
void interrupt_mli(void) interrupt 5{
	TF2=0;
	//LED=!LED;
	if(go==1){

		if(cpt==0){
			reg=65536.0-2212.0*intensite/100.0;
			RCAP2=reg;
			cpt+=1;
			LED=1;
		}
		else{
			reg=65536.0-2212.0*(100.0-intensite)/100.0;
			RCAP2=reg;		
			cpt=0;
			LED=0;
		}
	}else{
		LED=0;
	}
}

void interrupt_time(void) interrupt 16{
	T4CON&=0x7F;
	TEST=!TEST;
	Time_increment(); //incr�mentation toutes les ms
}

//-----------------------------------------------------------------------------
// Ins�rez vos fonctions d'interruption ici