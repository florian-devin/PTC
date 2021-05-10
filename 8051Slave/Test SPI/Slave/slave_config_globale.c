#include "slave_config_globale.h"
#include "c8051F020.h"
#include "c8051F020_SFR16.h"


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
	RCLK0 = 0;     // Source clock Timer 1
	TCLK0 = 0;
	PCON  |= 0x80; //SMOD0: UART0 Baud Rate Doubler Disabled.
	PCON &= 0xBF;  // SSTAT0=0
	SCON0 = 0x70;   // Mode 1 - Check Stop bit - Reception validee
	TI0 = 1;        // Transmission: octet transmis (pret e recevoir un char
					          // pour transmettre			
    ES0 = 1;        // interruption UART0 autorisee	
}

void cfg_UART1_mode1(void){
	PCON  |= (1<<4);
	//config uart
	SCON1 |= (1<<6);//Mode 1 8bit de data asynchrone
	SCON1 |= (1<<5);//Bit de Stop enable
    SCON1 |= (1<<4);//reception active

 // SCON1 |= (1<<1); //TI1 Transmission: octet transmis (pret a recevoir un char
					          // pour transmettre	
    EIE2  |= (1<<6); //ES1 interruption UART1 autorisee	
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
    XBR0  |= (1<<2); //Tx0 -> P0.0 & Rx0 -> P0.1
    //SPI
    XBR0  |= (1<<1); //SCK -> P0.2 | MISO -> P0.3 | MOSI -> P0.4 | NSS -> P0.5
    //UART1
    XBR2  |= (1<<2); //Tx1 -> P0.6 | Rx1 -> P0.7
    //PWM
    XBR0  |= (1<<4); //CEX0 -> P1.0 | CEX1 -> P1.1
}

void Port_IO_Init() {
    //GND                                        //B-0
    //3.3V                                       //A-0

    // P0.0  -  Tx        ,  Pull-push , Digital //C-12
    // P0.1  -  Rx        ,  Open-Drain, Digital //B-12
    // P0.2  -  SCK       ,  Open-Drain, Digital //A-12
    // P0.3  -  MISO      ,  Pull-push , Digital //C-11
    // P0.4  -  MOSI      ,  Open-Drain, Digital //B-11
    // P0.5  -  NSS       ,  Open-Drain, Digital //A-11
    // P0.6  -  Tx1       ,  Pull-push , Digital //C-10
    // P0.7  -  Rx1       ,  Open-Drain, Digital //B-10

    // P1.0  -  LED       ,  Push-Pull , PWM     //C-04
    // P1.1  -  CERVO V   ,  Push-Pull , PWM     //B-04
    // P1.2  -  Unassigned,  Open-Drain, Digital //A-04
    // P1.3  -  Unassigned,  Open-Drain, Digital //C-03
    // P1.4  -  Unassigned,  Open-Drain, Digital //B-03
    // P1.5  -  Unassigned,  Open-Drain, Digital //A-03
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
    P0MDOUT |= (1<<0); //Tx0
    P0MDOUT |= (1<<3); //MISO
    P0MDOUT |= (1<<6);//Tx1
    P1MDOUT |= (1<<0);//LED    
    P1MDOUT |= (1<<1);//CERVO V
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

    //Pour le cervo-moteur : CEX1

    PCA0CPM1    |= 0xC2; //PWM Enable
}

void Init_timer4() {
    //Utiliser comme horloge courante qui compte les ms
    // autoreload avec 43417 sans division par 12 pour avoir 1ms
    //Mode 1
    T4CON |= 0x04;
    EIE2 |= (1<<2);
    //interrupt 16

    //reload value
    RCAP4 = 0xA99A;// = 43418
}



void Init_Slave(void){
    Reset_Sources_Init();
    Oscillator_Init();
	Port_IO_Init();
    Init_crossbar();
    cfg_Clock_UART();
    cfg_UART0_mode1();
    Init_SPI();
    Init_PCA();
    Init_timer4();
}