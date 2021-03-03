// #define PTC_GETERCMD
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

//Decoupage de la trame en plusieur commande
//      - nom de la commande (D,E,Q,TV,A,B,S,RD,RG,RC,RA,G,ASS,MI,ME,IPO,POS,MOU,MOB,MOS,SD,L,LS,CS,PPH,SPH,AUX) get_commande()
//      - parametres (B,12,D:21, etc) get_param()
//get_commande(const char *str, char *commande) : place dans commande le nom de la commande recue
//get_param(const char *str,const unsigned char position, char *param) : place dans param le potition eme parametre qu'il soit complexe ou simple 
//get_complex_param(const char *param, char *name_param, char *value_param) : place le nom et la valeur du parametre dans name_param et valu_param

void get_commande(const char *str, char *commande);
void get_param(const char *str,const unsigned char position, char *param);
void get_complex_param(const char *param, char *name_param, char *value_param);