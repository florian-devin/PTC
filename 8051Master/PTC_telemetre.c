#include "TELEMETRE.h"

sbit trig_AV = P3^1;
sbit trig_AR = P3^2;
float d_AV;
float d_AR;

float MES_Dist_AV (void)
{
	return d_AV;
}

float MES_Dist_AR (void)
{
	return d_AR;
}

void sendTrig_AV (void)
{
	trig_AV = 1;
	Delay_10us();
	trig_AV = 0;	
}

void sendTrig_AR (void)
{
	trig_AR = 1;
	Delay_10us();
	trig_AR = 0;
}
}