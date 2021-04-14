#define CONFIG_GLOBALE


void Reset_Sources_Init();

void Oscillator_Init();

void Port_IO_Init();

void Init_crossbar() ;

void Init_interrupt(void);

void Init_SPI() ;

void Init_PCA(void);

void Init_timer4();

void Init_Device(void);