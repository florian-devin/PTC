#define CONFIG_GLOBALE


void Reset_Sources_Init();
void Port_IO_Init();
void Oscillator_Init_Osc_Quartz();
void Init_UART0();
void Init_interrupt(void);
void Init_Device(void);
//void Init_timer0(void);
void Init_timer2(void);
