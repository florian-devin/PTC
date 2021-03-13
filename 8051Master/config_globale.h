//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: config_globale.c
// Destination: 8150F020 Carte Master
// Description: Fonctions d'initialisation
//------------------------------------------------------

#define CONFIG_GLOBALE

#ifndef C8051F020_SFR16
	#include "c8051F020_SFR16.h"
#endif
#ifndef C8051F020
	#include "c8051F020.h"
#endif
#ifndef	CONFIG_GLOBALE
//	#include "config_globale.h"
#endif
#ifndef PTC_ACCUSEDERECEPTION
	#include "PTC_accuseDeReception.h"
#endif
#ifndef PTC_CONVERTION
	#include "PTC_convertion.h"
#endif
#ifndef PTC_GETERCMD
	#include "PTC_geter_cmd.h"
#endif
#ifndef PTC_STROPERATEURS
	#include "PTC_strOperateurs.h"
#endif
#ifndef PTC_TIMER
	#include "PTC_timer.h"
#endif
#ifndef PTC_UART
	#include "PTC_UART.h"
#endif


//Place le watchdog a DEAD (off)
//@param rien
//@return rien
void Reset_Sources_Init(void);

//initialise les ports
//@param rien
//@return rien
void Port_IO_Init(void);

//initialise sysclock avec le quartz externe
//@param rien
//@return rien
void Oscillator_Init_Osc_Quartz(void);

//initialise l'UART0 avec le Timer 1 en BaudRate generator (19200baud/s)
//@param rien
//@return rien
void Init_UART0(void);

//initialise l'UART1 avec le Timer 1 en BaudRate generator (19200baud/s)
//@param rien
//@return rien
void Init_UART1(void);

//initialise la liaison SPI 
//Fonction pas encore devellope
//@param rien
//@return rien
void Init_SPI(void);

//initialise les interuption
//@param rien
//@return rien
void Init_interrupt(void);

//initialise le device en appelant toutes les fonctions d'initialisation
//@param rien
//@return rien
void Init_Device(void);
