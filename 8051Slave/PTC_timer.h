// #define FREQ_CLK 22118300

// #define PTC_TIMER

// #ifndef C8051F020_SFR16
// 	#include "c8051F020_SFR16.h"
// #endif
// #ifndef C8051F020s
// 	#include "c8051F020.h"
// #endif
// #ifndef	CONFIG_GLOBALE
// 	#include "config_globale.h"
// #endif

// #ifndef PTC_ACCUSEDERECEPTION
// 	#include "PTC_accuseDeReception.h"
// #endif
// #ifndef PTC_CONVERTION
// 	#include "PTC_convertion.h"
// #endif
// #ifndef PTC_GETERCMD
// 	#include "PTC_geter_cmd.h"
// #endif
// #ifndef PTC_STROPERATEURS
// 	#include "PTC_strOperateurs.h"
// #endif
// #ifndef PTC_TIMER
// 	#include "PTC_timer.h"
// #endif
// #ifndef PTC_UART
// 	#include "PTC_UART.h"
// #endif

//#ifndef TIME_US
//	unsigned long int Time_us = 0;
//	#define TIME_US
//#endif


unsigned long int get_time_ms(void);
void Time_increment(void);
void Time_reset(void);
void Delay_1ms(void);
void Delay_1us(void);
