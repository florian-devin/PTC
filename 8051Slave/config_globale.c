/******************************************************************************
; 	FILE NAME  	: TP4_Lib_Config_Globale_8051F020.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configurations GLOBALES pour le 8051F020
*/
//******************************************************************************

#include "config_globale.h"
#include "c8051F020.h"
#include "c8051F020_SFR16.h"


void Reset_Sources_Init(){
    //Desactivation du Watchdog
	 WDTCN = 0xDE;
	 WDTCN = 0XAD;
}

void Oscillator_Init() {
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN    = 0x0C;
}


void Port_IO_Init() {
    // P0.0  -  LED       ,  Push-Pull , PWM
    // P0.1  -  CERVO V   ,  Push-Pull , PWM
    // P0.2  -  Unassigned,  Open-Drain, Digital
    // P0.3  -  Unassigned,  Open-Drain, Digital
    // P0.4  -  Unassigned,  Open-Drain, Digital
    // P0.5  -  Unassigned,  Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    // P2.0  -  Unassigned,  Open-Drain, Digital
    // P2.1  -  Unassigned,  Open-Drain, Digital
    // P2.2  -  Unassigned,  Open-Drain, Digital
    // P2.3  -  Unassigned,  Open-Drain, Digital
    // P2.4  -  Unassigned,  Open-Drain, Digital
    // P2.5  -  Unassigned,  Open-Drain, Digital
    // P2.6  -  Unassigned,  Open-Drain, Digital
    // P2.7  -  Unassigned,  Open-Drain, Digital

    // P3.0  -  Unassigned,  Open-Drain, Digital
    // P3.1  -  Unassigned,  Open-Drain, Digital
    // P3.2  -  Unassigned,  Open-Drain, Digital
    // P3.3  -  Unassigned,  Open-Drain, Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  Unassigned,  Open-Drain, Digital Input INT6
    // P3.7  -  Unassigned,  Open-Drain, Digital Input INT7
		
	// P4.0 to P7.7   Unassigned,  Open-Drain, Digital
    P0MDOUT |= (1<<0);
    P0MDOUT |= (1<<1);
}

void Init_interrupt(void){
    EA    = 1;      //Interuption General
}

void Init_SPI(void) {

}

void Init_PCA(void) {
	//sysclk divided by 12
    PCA0CN |= (1<<6);
    //Pour la lumiere : CEX0
	PCA0CPM0    |= (1<<1);  //PWM Enable

   // PCA0CN = 0x40;
    //Pour le cervo-moteur : CEX1
    PCA0CPM1    |= (1<<1); //PWM Enable
	
    XBR0        |= (1<<4); //Crossbar 
}



void Init_timer4() {
// autoreload avec 43417 sans division par 12 pour avoir 1ms
//Mode 1
T4CON |= 0x04;
EIE2 |= (1<<2);
//interrupt 16

//reload value
RCAP4 = 0xA99A;// = 43418

}

//-----------------------------------------------------------------------------
// Initialisation globale du Microcontroleur - 
//-----------------------------------------------------------------------------
void Init_Device(void) {
    XBR2  |= (1<<6); //Crossbar enable
    Reset_Sources_Init();
    Oscillator_Init();
    Port_IO_Init();
	 Init_PCA();
		Init_timer4();
    Init_interrupt();
}



