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
void Reset_Sources_Init();
void Port_IO_Init();
void Oscillator_Init_Osc_Quartz();
void Init_UART0();
void Init_interrupt(void);
void Init_Device(void);
