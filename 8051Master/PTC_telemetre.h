#ifndef __TELEMETRE__
#define __TELEMETRE__

#include "PTC_timer.h"
#include "config_globale.h"

float MES_Dist_AV (void);
int* get_bool_trig_AV(void);
int* get_bool_echo1_AV(void);
int* get_bool_echo2_AV(void);
float* get_T_AV(void);

#endif