//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.c
// Destination: 8150F020 Carte Slave
// Description: Fonctions qui permette de decomposer les commandes du 8051 Master
//------------------------------------------------------
#include "PTC_accuseDeReception_SPI.h"
#include "PTC_convertion.h"
#include "PTC_geter_cmd.h"
#include "PTC_servoMoteurVertical.h"
#include "PTC_strOperateurs.h"
#include "SPI_RingBuffer_Slave.h"
#include "PTC_pointeurLumineux.h"
#include "PTC_convertion.h"
#include "PTC_decodeCmd.h"
#include "UART1_RingBuffer_lib.h"
//------------------------------------------------------
//------------------------------------Variables Globales
extern unsigned char    temp_servo_V;
extern char             flag_print_arrive_servo_V;

//------------------------------------------------------
//------------------------------------------------------


void Cmd_epreuve_CS_Slave(const char *Pchaine_courante) { // Cmd = CS V [A:Angle]
    char str_param[8] = {0};
	char str_name[2]  = {0};
	char str_angle[4] = {0};
	char angle 		  = 0;

	get_param(Pchaine_courante,2,str_param);
	if (my_strlen(str_param) > 0) { //Parametres precis
		get_complex_param(str_param,str_name,str_angle);
		angle = my_atoi(str_angle);

		if (angle < 91 && angle > -91){
			temp_servo_V = (char)10*CDE_Servo_V(angle);
			flag_print_arrive_servo_V =1;
		}
	}
	else {//Parametre par default (0deg)
		temp_servo_V = (char)10*CDE_Servo_V(0);
		flag_print_arrive_servo_V = 1;
	}

}

void Cmd_epreuve_L_Slave(const char *Pchaine_courante) { 
	char str_param[8] = {0};
	int  num_param    = 1  ;
	int  intensite    = 100;
	int  duree_ON     = 99 ;
	int  duree_OFF    = 0  ;
	int  nb_allumage  = 1  ;

	do {
		get_param(Pchaine_courante,num_param,str_param);
		if (my_strlen(str_param) != 0){
			char str_value[4] = {0};
			char str_name[2]  = {0};
			num_param++;
			get_complex_param(str_param,str_name,str_value);
			if (my_strcmp(str_name, "I")){
				int value = my_atoi(str_value);
				intensite = value;
			}
			else if  (my_strcmp(str_name, "D")) {
				int value = my_atoi(str_value);
				duree_ON = value;
			}
			else if  (my_strcmp(str_name, "E")) {
				int value = my_atoi(str_value);
				duree_OFF = value;
			}
			else if  (my_strcmp(str_name, "N")) {
				int value = my_atoi(str_value);
				nb_allumage = value;
			}
			else {
				AR_cmd_incorrecte_SPI();
				return;
			}
		} 
	} while (my_strlen(str_param) != 0);
	
	Lumiere(intensite,duree_ON*100,duree_OFF*100,nb_allumage); //Le *100 permet de conevrtir les 100ms en ms
}

void Cmd_epreuve_LS_Slave(void) {//Commande pour la carte esclave
	Lumiere_Stop();
}


void Cmd_epreuve_ASS_Slave(const char *Pchaine_courante) {
	my_strcat(Pchaine_courante,"\r");
	//serOutchar_UART0(Pchaine_courante); 
}

void Cmd_epreuve_SD_Slave(const char *Pchaine_courante) {
	my_strcat(Pchaine_courante,"\r");
	//serOutchar_UART0(Pchaine_courante); 
}


void Cmd_epreuve_PPH_Slave(const char *Pchaine_courante_SPI){//Commande pour la Raspberry
	my_strcat(Pchaine_courante_SPI,"\r");
	serOutstring_uart1(Pchaine_courante_SPI);
}

void Cmd_epreuve_SPH_Slave(void) {//Commande pour la Raspberry
	serOutstring_uart1("SPH\r");
}












