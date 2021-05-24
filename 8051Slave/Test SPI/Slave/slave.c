//Fichier de test pour dev la liaison SPI 



#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "slave_config_globale.h"
#include "SPI_RingBuffer_Slave.h"
#include "UART0_RingBuffer_lib.h"
#include "UART1_RingBuffer_lib.h"
#include "PTC_SPI.h"
#include "PTC_strOperateurs.h"
#include "PTC_geter_cmd.h"
#include "PTC_decodeCmd.h"
#include "PTC_timer.h"
#include "PTC_pointeurLumineux.h"
#include "PTC_PWM.h"


#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK           22118400 //approximate SYSCLK frequency in Hz
#endif


void decodage_commande(char *Pchaine_courante_SPI);
void envoie_info_confirmation(void);
//--------------------------Variables Globales
//En lien avec la SPI
char chaine_courante_SPI[64] = {0};//chaine total qui vas contenir le mot recu

//En lien avec le pointeur lumineu
char            Lumiere_loop_Enable = 0; //1 si il faut faire Lumiere_loop 0 sinon
unsigned int   Lumiere_Intensite, Lumiere_Lum_Nbre;
unsigned int    Lumiere_Lum_ON, Lumiere_Lum_OFF;

//En lien avec le cervo moteur vertical
unsigned char    temp_servo_V;
char             flag_print_arrive_servo_V = 0;
unsigned long 	last_time_capture_servo_V 	= 0; //dernier temp capture en ms
//-------------------Declaration des ports E/S

//--------------------------------------------
//------------------------------Configurations



//-------------------------------------------


//-----------------------Programme principale


void setup() {
    Init_Slave();
    init_Serial_Buffer();//UART0
	init_Serial_Buffer_uart1();
    init_Serial_Buffer_SPI();
    EA    = 1; //interruption general enable
}


void loop() {
    if (Rx_chaine_SPI(chaine_courante_SPI)) {
        decodage_commande(chaine_courante_SPI);
    }

    if (Lumiere_loop_Enable) {//Routine alumage lumiere
        Lumiere_loop();
    }
    envoie_info_confirmation();
}

void decodage_commande(char *Pchaine_courante_SPI){ //fonction qui decode les commades et les applique
	char commande[7] = {0};
	get_commande(Pchaine_courante_SPI,commande);

	if (my_strcmp(commande,"CS")) 
		Cmd_epreuve_CS_Slave(Pchaine_courante_SPI);
	else if (my_strcmp(commande,"L"))
		Cmd_epreuve_L_Slave(Pchaine_courante_SPI);
	else if (my_strcmp(commande,"LS"))
		Cmd_epreuve_LS_Slave(); 
	else if (my_strcmp(commande,"ASS"))
		Cmd_epreuve_ASS_Slave(Pchaine_courante_SPI);
	else if (my_strcmp(commande,"SD"))
		Cmd_epreuve_SD_Slave(Pchaine_courante_SPI);
	else if (my_strcmp(commande,"PPH"))
		Cmd_epreuve_PPH_Slave(Pchaine_courante_SPI);
	else if (my_strcmp(commande,"SPH"))
		Cmd_epreuve_SPH_Slave();
	else if (my_strcmp(commande,"SD"))
		Cmd_epreuve_SD_Slave(Pchaine_courante_SPI);
	else if (my_strcmp(commande,"SPI")) { //Pour le test de la liaison SPI
		serOutstring("Commande SPI bien recu !\r\n");
        serOutstring_SPI("Cmd SPI ok");
	} 

	RAZ_str(Pchaine_courante_SPI);
}

void envoie_info_confirmation(void){
	if (flag_print_arrive_servo_V == 1) {
		int tmp = temp_servo_V - (get_time_ms() - last_time_capture_servo_V); //TODO: remouve this variable
		if(temp_servo_V - (get_time_ms() - last_time_capture_servo_V) <= 0) {
			serOutstring_SPI("SPICSV\r");
			flag_print_arrive_servo_V = 0;
		}
		else {
			temp_servo_V -= (get_time_ms() - last_time_capture_servo_V);
			last_time_capture_servo_V = get_time_ms();
		}
	}
}


void startup() {
    serOutstring("UART Slave Ready \r\n");
 	while(serInchar_SPI() != 0x01);
	serOutchar_SPI(0x02);
	while(serInchar() != 't'){//Stm32
		serOutchar('s');
		Delay(1);
	}
	serOutchar_SPI(0x03);
	while(serInchar_uart1() != 'b'){ //Rasp
		serOutchar_uart1('a');
		Delay(1);
	}
	serOutchar_SPI(0x04);
	analogWrite_CEX1(3692);
} 


void main(){
    setup();
    startup();
    while(1) {
        loop();
    }
}


