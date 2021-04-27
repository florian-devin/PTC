//Fichier de test pour dev la liaison SPI 



#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "SPI_RingBuffer_Slave.h"
#include "UART0_RingBuffer_lib.h"

#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK           22118400 //approximate SYSCLK frequency in Hz
#endif

//--------------------------Variables Globales

//-------------------Declaration des ports E/S

//--------------------------------------------
//------------------------------Configurations


void cfg_Clock_UART(void) {
  CKCON |= 0x10;      // T1M: Timer 1 use the system clock.
  TMOD |= 0x20;       //  Timer1 auto reload
	TMOD &= 0x2f;			  // Timer1 configure en timer 8 bit avec auto-reload	
	TF1 = 0;				  // Flag Timer efface

  TH1 = 0xB8;
	//TH1 = -(Preload_Timer0);
	ET1 = 0;				   // Interruption Timer 1 devalidee
	TR1 = 1;				   // Timer1 demarre
}

void cfg_UART0_mode1(void) {
    init_Serial_Buffer();//init des buffers
	RCLK0 = 0;     // Source clock Timer 1
	TCLK0 = 0;
	PCON  |= 0x80; //SMOD0: UART0 Baud Rate Doubler Disabled.
	PCON &= 0xBF;  // SSTAT0=0
	SCON0 = 0x70;   // Mode 1 - Check Stop bit - Reception validee
	TI0 = 1;        // Transmission: octet transmis (pret e recevoir un char
					          // pour transmettre			
    ES0 = 1;        // interruption UART0 autorisee	
}

void Reset_Sources_Init(){
    //Desactivation du Watchdog
	 WDTCN = 0xDE;
	 WDTCN = 0XAD;
}

void Oscillator_Init() {
    int i  = 0;
    OSCXCN = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN = 0x0C;
}

void Init_crossbar() {
    XBR2  |= (1<<6); //Crossbar enable
    //UART0
    XBR0  |= (1<<2); //Tx -> P0.0 & Rx -> P0.1
    //SPI
    XBR0  |= (1<<1); //SCK -> P0.2 | MISO -> P0.3 | MOSI -> P0.4 | NSS -> P0.5
}

void Port_IO_Init() {


    // P0.0  -  Tx        ,  Pull-push , Digital
    // P0.1  -  Rx        ,  Open-Drain, Digital
    // P0.2  -  SCK       ,  Open-Drain, Digital
    // P0.3  -  MISO      ,  Pull-push , Digital
    // P0.4  -  MOSI      ,  Open-Drain, Digital
    // P0.5  -  NSS       ,  Open-Drain, Digital
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
    P0MDOUT |= (1<<0); //Tx
    P0MDOUT |= (1<<3); //MISO
}

void Init_SPI() {
    //Config de l'horloge
    SPI0CFG &= ~(0xC0); //Polarite et etat horloge

    //Actication de SPI
    SPIEN = 1;

    //Activation de l'interruption 
    EIE1 |= (1<<0);
}
//-------------------------------------------


//-----------------------Programme principale


void setup() {
    Reset_Sources_Init();
    Oscillator_Init();
    Init_crossbar();
    Port_IO_Init();
    cfg_Clock_UART();
    cfg_UART0_mode1();
    Init_SPI();
    init_Serial_Buffer_SPI();
    EA    = 1; //interruption general enable
}


void loop() {
	char caractere = 0;
    char caractere1 = 0;
    while ((caractere=serInchar_SPI())!=0) serOutchar(caractere);
    while ((caractere1=serInchar())!=0) serOutchar_SPI(caractere1);
}


void startup() {
    serOutstring("UART0 Ready !");
		while(serInchar_SPI() != 0x01);
		serOutchar_SPI(0x02);
		serOutstring("Slave ready \r");
} 


void main(){
    setup();
    startup();
    while(1) {
        loop();
       
    }
}


//----------------------------------------------

//-----------------------Fonction d'interruption


//----------------------------------------------