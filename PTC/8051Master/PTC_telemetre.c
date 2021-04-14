#include "PTC_telemetre.h"

sbit trig_AV = P3^1;
int bool_trig_AV = 1;
int bool_echo1_AV = 0;
int bool_echo2_AV = 0;
float T_AV;

float MES_Dist_AV (void)
{
	if (bool_trig_AV == 1)
	{	
		trig_AV = 1;
		Delay_10us();
		trig_AV = 0;
		bool_trig_AV = 0;
	} 
	if (bool_echo1_AV == 1)
	{
		if (bool_echo2_AV == 1)
		{
			bool_trig_AV = 1;
			bool_echo2_AV = 0;
			bool_echo1_AV = 0;
			T_AV = T_AV/58;
			return T_AV; // Distance en cm
		} else 
		{
			return MES_Dist_AV();
		}
	} else
	{
		return MES_Dist_AV();
	}
}

int* get_bool_trig_AV(void)
{
	return &bool_trig_AV;
}

int* get_bool_echo1_AV(void)
{
	return &bool_echo1_AV;
}

int* get_bool_echo2_AV(void)
{
	return &bool_echo2_AV;
}

float* get_T_AV(void)
{
	return &T_AV;
}