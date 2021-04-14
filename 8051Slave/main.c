//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: main.c
// Destination: 8150F020 Carte Slave
// Description: Programme principal de la carte slave
//------------------------------------------------------


#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "config_globale.h"
#include "PTC_timer.h"
#include "PTC_pointeurLumineux.h"
#include "PTC_PWM.h"
#include "PTC_SPI.h"
#include "PTC_geter_cmd.h"
#include "PTC_strOperateurs.h"
#include "PTC_accuseDeReception_SPI.h"

#include "PTC_decodeCmd.h"


#include "UART0_RingBuffer_lib.h"
#include "UART1_RingBuffer_lib.h"
#include "SPI_RingBuffer_Slave.h"

//En lien avec le pointeur lumineu
char            Lumiere_loop_Enable = 0; //1 si il faut faire Lumiere_loop 0 sinon
unsigned char   Lumiere_Intensite, Lumiere_Lum_Nbre;
unsigned int    Lumiere_Lum_ON, Lumiere_Lum_OFF;

//En lien avec la SPI
char chaine_courante_SPI[64] = {0};//chaine total qui vas contenir le mot recu

//En lien avec le cervo moteur vertical
unsigned char    temp_servo_V;
char             flag_print_arrive_servo_V = 0;
unsigned long 	last_time_capture_servo_V 	= 0; //dernier temp capture en ms
//Headers
void setup();
void startup();
void loop();
void decodage_commande(char *Pchaine_courante_SPI);
void envoie_info_confirmation(void);


void setup(void) {
	char c;
  Init_Device();
	
	while(c=serInchar_SPI() == 0);
	serOutstring("Slave ready \r");
	RAZ_str(chaine_courante_SPI);
}

void loop(void) {
    if (Rx_chaine_SPI(chaine_courante_SPI)) {
        decodage_commande(chaine_courante_SPI);
    }

    if (Lumiere_loop_Enable) {//Routine alumage lumiere
        Lumiere_loop();
    }
    
}

void main(void) {
    setup();
    while(1){
        loop();
    }
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
	else if (my_strcmp(commande,"SPI")) { //Pour le test de la liaison SPI
		//TODO vecteur de test de spi
	} 
	else 
		AR_cmd_incorrecte_SPI();

	RAZ_str(Pchaine_courante_SPI);
}

void envoie_info_confirmation(void){
	if (flag_print_arrive_servo_V == 1) {
		if(temp_servo_V - (get_time_ms() - last_time_capture_servo_V) <= 0) {
			serOutstring_SPI("AS V\r");
			flag_print_arrive_servo_V = 0;
		}
		else {
			temp_servo_V -= (get_time_ms() - last_time_capture_servo_V);
			last_time_capture_servo_V = get_time_ms();
		}
	}
}


void Interrupt_Time(void) interrupt 16 {//interruption declancher par l'overflow du Timer 3 (toutes les ms)
	T4CON &= ~(1<<7); //interrupt flag
    Time_increment();
}