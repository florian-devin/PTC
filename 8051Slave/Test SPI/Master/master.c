//Fichier de test pour dev la liaison SPI 



#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "SPI_RingBuffer_lib.h"

#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK           22118400 //approximate SYSCLK frequency in Hz
   #define SCK              50000    // Frequence de la liaison SPI
   #define SPACE_TRAME      100      //espacement tres trames (en nb de trame exemple si SPACE_TRAME = 100, il y a un trame toutes les 100 periode de trame)
   #define T_TRAME_SPI      (8*(1/SCK))
   #define T_RECURRENCE_SPI (SPACE_TRAME*T_TRAME_SPI)
   #define T_T3             (T_RECURRENCE_SPI*SYSCLK)
#endif


//-------------------Declaration des ports E/S

//--------------------------------------------
//------------------------------Configurations

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

void Init_Timer3(void) {
    //TMR3RL  = 0xFFFF - T_T3; //valeur de reload
    TMR3RL  = 0xFFFF - 353894; //valeur de reload
    TMR3CN |= (1<<2); //Timer 3 enable
    TMR3CN |= (1<<1); //SYSCLK/1

    EIE2   |= (1<<0); //ISR enable
}

void Init_crossbar() {
    //SPI
    XBR0 |= (1<<1); //SCK -> P0.0 | MISO -> P0.1 | MOSI -> P0.2 | NSS -> P0.3
}

void Port_IO_Init() {
    // P0.0  -  SCK       ,  Push-Pull , Digital
    // P0.1  -  MISO      ,  Open-Drain, Digital
    // P0.2  -  MOSI      ,  Push-Pull , Digital
    // P0.3  -  NSS       ,  Open-Drain, Digital
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

}

void Init_SPI() {
    //Config de l'horloge
    SPI0CFG &= ~(0xC0); //Polarite et etat horloge
    SPI0CKR = SYSCLK / (2 * SCK) - 1; //fixe la frequence de SCK

    //Actication de master
    MSTEN = 1;

    //nb de bit de shift
    SPI0CFG |= 0x07; //8bits

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
    Init_SPI();
    init_Serial_Buffer_SPI();
    Init_Timer3();
    EA    = 1; //interruption general enable
}


void loop() {

}


void startup() {
    serOutchar_SPI('s');
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

void Timer3_ISR(void) interrupt 14 {
    TMR3CN &= ~(1<<7); //flag 
    SPIF = 1; //declanchement de l'ISR de SPI pour envoyer un caractere si il y en a dans le buffer 
}

//----------------------------------------------