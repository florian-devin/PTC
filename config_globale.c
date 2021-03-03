/******************************************************************************
; 	FILE NAME  	: TP4_Lib_Config_Globale_8051F020.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configurations GLOBALES pour le 8051F020
*/
//******************************************************************************

#include "config_globale.h"


void Reset_Sources_Init(){
    //Desactivation du Watchdog
	 WDTCN = 0xDE;
	 WDTCN = 0XAD;
}


void Port_IO_Init() {
    // P0.0  -  Tx, 				 Pull-push, Digital
    // P0.1  -  Rx, 				 Open-Drain, Digital
    // P0.2  -  Unassigned,  Open-Drain, Digital
    // P0.3  -  Unassigned,  Open-Drain, Digital
    // P0.4  -  Unassigned,  Open-Drain, Digital
    // P0.5  -  Unassigned,  Open-Drain, Digital
    // P0.6  -  Tx1,         Pull-push, Digital
    // P0.7  -  Rx1,         Open-Drain, Digital

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

		P0MDOUT |= (1<<0); //P0.0
		P0MDOUT |= (1<<6); //P0.6
}

//-----------------------------------------------------------------------------
// Config oscillateur - SYSCLK = 22,1184MHz - Oscillateur externe à quartz 
//-----------------------------------------------------------------------------
void Oscillator_Init_Osc_Quartz(){
		int i = 0;
    OSCXCN    = 0x67;  // Config de l'horloge externe - Quartz > 6,7 MHz
    for (i = 0; i < 3000; i++){} // attente stabilisation Fosc quartz
    while ((OSCXCN & 0x80) == 0); // validation stabilite du quartz
    OSCICN    = 0x0C;  // Commutation sur oscillateur externe 
	                     // L'oscillateur n'est pas stopp
}

//-----------------------------------------------------------------------------
// Initialisation de UART0
//-----------------------------------------------------------------------------
void Init_UART0(){
    //initialisation du Timer1 en mode 8-bit autoreload
    TMOD |= (1<<5);  //Mode 2 (8-bit autoreload)
    TMOD &= ~(1<<4); //Mode 2 (8-bit autoreload)
    TH1   = 0xFA;    //Reload value (pour avoir 19200 bauds)
    TR1   = 1;       //Timer1 Enable

    //Config Crossbar
		XBR2 |= (1<<6); //Crossbar enable
    XBR0 |= (1<<2); //Tx -> P0.0 & Rx -> P0.1

    //Config de UART
    PCON |= (1<<7);  //Doubleur BaudRate (pour avoir 19200 bauds)
    //SCON0
    SM00 = 0; //Mode 1 8bit de data asynchrone
    SM10 = 1; //Mode 1 8bit de data asynchrone
    SM20 = 1; //Bit de Stop enable
}

void Init_UART1(){
	//crossbar
	//Tx1 = P0.6  Rx = P0.7
	XBR2 |= (1<<2);
	
	//config uart
	SCON1 |= (1<<7);//Mode 1 8bit de data asynchrone
	SCON1 |= (1<<6);//Mode 1 8bit de data asynchrone
	SCON1 |= (1<<5);//Bit de Stop enable
}

void Init_SPI(){//fct a developper plus tard
	//crossbar 
	//P0.2 P0.3 P0.4 P0.5 
	XBR0 |= (1<<1);
}
//-----------------------------------------------------------------------------
// Initialisation des interuption
//-----------------------------------------------------------------------------
void Init_interrupt(void){
    ES0   = 1;      //UART0
		EIE2 |= (1<<6); //UART1
    EA    = 1;      //Interuption General
}


/*
void Init_Timer2(void){
  CKCON = CKCON | (1<<5);//T2M a 1 (horloge interne (a 2MHz))
  TR2 = 1;//activation de Timer2
}
*/

//-----------------------------------------------------------------------------
// Initialisation globale du Microcontroleur - 
//-----------------------------------------------------------------------------
void Init_Device(void) {
    Reset_Sources_Init();
    Port_IO_Init();
    Oscillator_Init_Osc_Quartz();
    Init_UART0();
		Init_UART1();
		Init_SPI();
    //Init_Timer2();
    Init_interrupt();
}



