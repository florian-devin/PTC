

// #define PTC_UART

// #ifndef C8051F020_SFR16
// 	#include "c8051F020_SFR16.h"
// #endif
// #ifndef C8051F020
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
void Flag_TX0_fct(void);
void Send_str(char *str);
void Send_char(const char caractere);
int Rx_chaine(char *Pchaine_courante);
void caractere_recu_fct_uart0(char Pcaractere_recu);


void caractere_recu_fct_uart1(char Pcaractere_recu);
void Send_char_uart1(const char caractere);
void Send_str_uart1(char *str);
void Flag_TX1_fct(void);