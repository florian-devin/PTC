sbit trig_AV = P3^1;
sbit trig_AR = P3^2;
int bool_trig = 1;
int bool_echo1 = 0;
int bool_echo2 = 0;
int bool_out_distance = 0;
float T;

float MES_Dist_AV (void)
{
	if (bool_trig != 0)
	{	
		trig_AV = 1;
		Delay_10us();
		trig_AV = 0;
		bool_trig = 0;
		EIE2 |= 0x10; // Autorisation interruption INT6
	} 
	if (bool_out_distance != 0)
	{
		bool_trig = 1;
		return -1.0;
	}
	if (bool_echo1 != 0)
	{
		if (bool_echo2 != 0)
		{
			bool_trig = 1;
			bool_echo2 = 0;
			bool_echo1 = 0;
			T = T*11.688/58 - 1.13;
			return T; // Distance en cm
		} else 
		{
			return MES_Dist_AV();
		}
	} else
	{
		return MES_Dist_AV();
	}
}

float MES_Dist_AR (void)
{
	if (bool_trig != 0)
	{	
		trig_AR = 1;
		Delay_10us();
		trig_AR = 0;
		bool_trig = 0;
		EIE2 |= 0x10; // Autorisation interruption INT6
	} 
	if (bool_out_distance != 0)
	{
		bool_trig = 1;
		return -1.0;
	}
	if (bool_echo1 != 0)
	{
		if (bool_echo2 != 0)
		{
			bool_trig = 1;
			bool_echo2 = 0;
			bool_echo1 = 0;
			T = T*11.688/58 - 1.13;
			return T; // Distance en cm
		} else 
		{
			return MES_Dist_AV();
		}
	} else
	{
		return MES_Dist_AV();
	}
}
