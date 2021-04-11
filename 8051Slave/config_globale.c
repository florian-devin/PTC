/******************************************************************************
; 	FILE NAME  	: TP4_Lib_Config_Globale_8051F020.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configurations GLOBALES pour le 8051F020
*/
//******************************************************************************

#include "config_globale.h"
#include "c8051F020.h"
#include "c8051F020_SFR16.h"

#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK           22118400 //approximate SYSCLK frequency in Hz
#endif

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
    //UART 0 pour communication avec stm32
    //UART 1 pour communication avec le module radio (FPGA)
    //SPI pour communication avec 8051Master

    // P0.0  -  Tx0       ,  Pull-push , Digital
    // P0.1  -  Rx0       ,  Open-Drain, Digital

    // P0.2  -  SCK       ,  Open-Drain, Digital
    // P0.3  -  MISO      ,  Pull-push , Digital
    // P0.4  -  MOSI      ,  Open-Drain, Digital
    // P0.5  -  NSS       ,  Open-Drain, Digital

    // P0.6  -  Tx1       ,  Pull-push , Digital
    // P0.7  -  Rx1       ,  Open-Drain, Digital

    // P1.0  -  LED       ,  Push-Pull , PWM
    // P1.1  -  CERVO V   ,  Push-Pull , PWM
   
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
    P0MDOUT |= (1<<3);
    P0MDOUT |= (1<<6);
    
    P1MDOUT |= (1<<0);
    P1MDOUT |= (1<<1);
}

void Init_crossbar() {
    XBR2  |= (1<<6); //Crossbar enable
    //UART0
    XBR0  |= (1<<2); //Tx -> P0.0 & Rx -> P0.1
    //SPI
    XBR0  |= (1<<1); //SCK -> P0.2 | MISO -> P0.3 | MOSI -> P0.4 | NSS -> P0.5
    //UART1
    XBR2  |= (1<<2); //Tx -> P0.6 & Rx -> P0.7
}


void Init_interrupt(void){
    EA    = 1;      //Interuption General
}

void Init_SPI() {
    //Config de l'horloge
    SPI0CFG &= ~(0xC0); //Polarite et etat horloge

    //Actication de SPI
    SPIEN = 1;

    //Activation de l'interruption 
    EIE1 |= (1<<0);
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
    Reset_Sources_Init();
    Oscillator_Init();
    Port_IO_Init();
	Init_PCA();
	Init_timer4();
    Init_interrupt();
    Init_crossbar();
    Init_SPI();
    init_Serial_Buffer_SPI();
}



