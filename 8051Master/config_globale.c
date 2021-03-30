//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: config_globale.c
// Destination: 8150F020 Carte Master
// Description: Fonctions d'initialisation
//------------------------------------------------------

#include "config_globale.h"
#include "UART1_RingBuffer_lib.h"
#include "UART0_RingBuffer_lib.h"
#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK 22118400 //approximate SYSCLK frequency in Hz
   #define BAUDRATE  19200L          // Baud rate of UART in bps
                                   // Le caractère 'L' force l'évaluation de BAUDRATE en entier long
#endif


extern char Flag_RX1;
sbit INT6 = P3^6;

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
    XBR1 |= 0x40; // Validation crossbar T2EX
    XBR2 |= 0x40; //enable le crossbar
    XBR0 |= 0x08; // route le signal CEX0 sur un port pin (servo) 
    P3MDOUT |= 0x02; //Configuration P3.1 en push-pull
	  INT6 = 1; // Configuration de P3.6 en input
	  // Sensibilité de /INT6 initialement mise a front montant
	  P3IF |= 0x04;
	  P3IF &= 0xBF;
}

//-----------------------------------------------------------------------------
// Config oscillateur - SYSCLK = 22,1184MHz - Oscillateur externe � quartz 
//-----------------------------------------------------------------------------
/*void Oscillator_Init_Osc_Quartz(){
		int i = 0;
    OSCXCN    = 0x67;  // Config de l'horloge externe - Quartz > 6,7 MHz
    for (i = 0; i < 3000; i++){} // attente stabilisation Fosc quartz
    while ((OSCXCN & 0x80) == 0); // validation stabilite du quartz
    OSCICN    = 0x0C;  // Commutation sur oscillateur externe 
	                     // L'oscillateur n'est pas stopp
}*/

void Oscillator_Init() {
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN    = 0x0C;
}


#define Preload_Timer0 (SYSCLK/(BAUDRATE*16))
#if Preload_Timer0 > 255 
#error "Valeur Preload Timer0 HORS SPECIFICATIONS"
#endif 
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
	    //Config Crossbar
		XBR2 |= (1<<6); //Crossbar enable
    XBR0 |= (1<<2); //Tx -> P0.0 & Rx -> P0.1
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
  init_Serial_Buffer_uart1();//buffers
	//crossbar
  XBR2  |= (1<<6); //Crossbar enable
	XBR2  |= (1<<2);//Tx1 = P0.6  Rx = P0.7
	
	PCON  |= (1<<4);
	//config uart
	SCON1 |= (1<<6);//Mode 1 8bit de data asynchrone
	SCON1 |= (1<<5);//Bit de Stop enable
  SCON1 |= (1<<4);//reception active

 // SCON1 |= (1<<1); //TI1 Transmission: octet transmis (pret a recevoir un char
					          // pour transmettre	
  EIE2  |= (1<<6); //ES1 interruption UART1 autorisee	
}

void Init_SPI(){//fct a developper plus tard
	//crossbar 
	//P0.2 P0.3 P0.4 P0.5 
	XBR0 |= (1<<1);
}

void Init_Timer4() {
// autoreload avec 43417 sans division par 12 pour avoir 1ms
//Mode 1

//reload value
RCAP4 = 43417;
}
//-----------------------------------------------------------------------------
// Initialisation des interuption
//-----------------------------------------------------------------------------
void Init_interrupt(void){
    ES0   = 1;      //UART0
		EIE2 |= (1<<6); //UART1
    EA    = 1;      //Interuption General
    EIE2 |= 0x10; // Interruption INT6
    ET2 = 1; // Interruption overflow Timer2
}

void Init_Timer2(void){
    RCLK0 = 0;
    TCLK0 = 0;
    CPRL2 = 1;
    TR2 = 1;
    EXEN2 = 1;
}

//PCA pour le servomoteur 

void Init_PCA()
{
	PCA0MD = 0x00; //sysclk divided by 12
	PCA0CN |= 0x41; // on choisit le module 0 ( bit 0 mit à 1) et enable PCA counter timer
	PCA0CPM0 = 0xC2; // mode PWM 16 bits et on enable le PWM0

	// Registres de rechargement 
	PCA0CPH0 = 0; //On initialise.Pour déterminer le duty high
	PCA0CPL0 = 0;
	
}

//-----------------------------------------------------------------------------
// Initialisation globale du Microcontroleur - 
//-----------------------------------------------------------------------------
void Init_Device(void) {
    Reset_Sources_Init();
    Port_IO_Init();
    Oscillator_Init();
    //Oscillator_Init_Osc_Quartz();
    Init_SPI();
    cfg_Clock_UART();
    cfg_UART0_mode1();
    cfg_UART1_mode1();
		
    //Init_Timer2();
    Init_interrupt();
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////Initialisation du robot

void Init_Robot(){
  Robot_restore();
}

void Robot_restore(){
  serOutstring_uart1("restore\r");
  while (serInchar_uart1()==0);
  Delay(10);
}

