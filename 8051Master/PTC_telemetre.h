#ifndef __TELEMETRE__
#define __TELEMETRE__

#include "PTC_timer.h"
#include "TP4_Lib_Config_Globale_8051F020.H"

float MES_Dist_AV (void);
float MES_Dist_AR (void);
void sendTrig_AV (void);
void sendTrig_AR (void);
#endif