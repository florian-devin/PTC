//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: main.c
// Destination: 8150F020 Carte Master
// Description: Fonctions main
//------------------------------------------------------

#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "config_globale.h"
#include "UART0_RingBuffer_lib.h"
#include "UART1_RingBuffer_lib.h"
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_geter_cmd.h"
#include "PTC_strOperateurs.h"
#include "PTC_timer.h"
#include "PTC_UART.h"
#include "PTC_deplacement.h"
#include "PTC_math.h"
#include "PTC_detection.h"
#include "PTC_telemetre.h"



//-----------------------------------------------------------------------------
//variables globales
//En lien avec l'UART0
//char interrupt_out = 0;        //mon flag pour le caractere recu
//char caractere_recu = '\0';    //caractere qui vient d'etre recu
char chaine_courante[64] = {0};//chaine total qui vas contenir le mot recu (20 caractere max)

int epreuve_enable = 0; //Flag qui pas a 1 si l'utilisateur a donné le debut de l'epreuve


int vitesse_par_default = 20; //vitesse du robot par default (modifie par la cmd TV vitesse)


char test[2] = {0}; //a supprimer 

/*
Variables globales en lien avec le telemetre
*/

sbit commandCapture = P3^3; // Commande la capture du timer2
int measureCycle = 1; // Compte le nombre d'overflow du Timer 2


//En lien avec le temp
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//headers
//void main();
void setup();
void startup();
void loop();
void decodage_commande(char *Pchaine_courante);
//ceci est un com
//---------------------------------------------Fct pour le debug
void my_println(const char *str){
	serOutstring(str);
	serOutstring("\r\n");
}
void my_print(const char *str){
	serOutstring(str);
}
//-----------------------------------------------------------------------------


void main(){
    setup();
    startup();
    while(1) {
        loop();
    }
}


void setup(){
  Init_Device(); //fonction de config_globale.c
  Init_Robot(); //rst de la carte serial etc...
}


void startup(){
	my_println("go");
}
	
void loop() {
	if (Rx_chaine(chaine_courante) == 1){
		decodage_commande(chaine_courante);
	}

}

void decodage_commande(char *Pchaine_courante){ //fonction qui decode les commades et les applique
	char commande[7] = {0};
	get_commande(Pchaine_courante,commande);

	if (my_strcmp(commande,"D")){
		char str_type_epreuve[2] = {0};
		get_param(Pchaine_courante, 1, str_type_epreuve);
		my_strcat(test, str_type_epreuve);
		if (my_strlen(str_type_epreuve) > 0) {//il y a un parametre
			int type_epreuve = my_atoi(str_type_epreuve);
			if (type_epreuve > 0 && type_epreuve < 9) {
				AR_cmd_correcte();
				epreuve_enable = type_epreuve;
			}
			else
				AR_cmd_incorrecte();
		}
		else {
			AR_cmd_correcte();
			epreuve_enable = 1;
		}
	}

	else if (my_strcmp(commande,"E")){
		char str_type_epreuve[2] = {0};
		get_param(Pchaine_courante, 1, str_type_epreuve);
		AR_cmd_correcte();
		epreuve_enable = 0;
	}

	else if (epreuve_enable > 0) {
		if (my_strcmp(commande,"A")){ //Avancer
			char str_param[12] = {0};
			int param = 0;
			get_param(Pchaine_courante,1, str_param);
			if (my_strlen(str_param) > 0) {
				param = my_atoi(str_param);
				if (param <= 100 && param >= 5){
					AR_cmd_correcte();
					Avancer(str_param);
				}
				else {
					AR_cmd_incorrecte();
				}
			}
			else {
				char str_vitesse_par_default[3] = {0};
				AR_cmd_correcte();
				Avancer(my_itoa(vitesse_par_default,str_vitesse_par_default));
			}
		}

		else if (my_strcmp(commande,"B")){//Reculer
			char str_param[12] = {0};
			int param = 0;
			get_param(Pchaine_courante,1, str_param);
			if (my_strlen(str_param) > 0) {
				param = my_atoi(str_param);
				if (param <= 100 && param >= 5) {
					AR_cmd_correcte();
					Reculer(str_param);
				}
				else
					AR_cmd_incorrecte();
			}
			else {
				char str_vitesse_par_default[3] = {0};
				AR_cmd_correcte();
				Reculer(my_itoa(vitesse_par_default,str_vitesse_par_default));
			}
			
		}

		else if (my_strcmp(commande,"RG")){ //Rotation gauche
			AR_cmd_correcte();
			turn_left(90);
		}

		else if (my_strcmp(commande,"RD")){ //Rotation droite
			AR_cmd_correcte();
			turn_right(90);
		}

		else if (my_strcmp(commande,"RC")){  //Rotation complete
			char str_param[12] = {0};
			get_param(Pchaine_courante,1, str_param);
			if (my_strcmp(str_param,"D")){
				AR_cmd_correcte();
				turn_right(180);
			}
			else if (my_strcmp(str_param,"G")){
				AR_cmd_correcte();
				turn_left(180);
			}
			else 
				AR_cmd_incorrecte();
		}

		else if (my_strcmp(commande,"RA")){ //Rotation d'un certain angle
			char str_param[8] = {0};
			char str_angle[4] = {0};
			char direction[2] = {0};
			get_param(Pchaine_courante,1,str_param);
			get_complex_param(str_param,direction,str_angle);
			if (my_strcmp(direction,"D")){
				AR_cmd_correcte();
				turn_right(my_atoi(str_angle));
			}
			else if (my_strcmp(direction,"G")){
				AR_cmd_correcte();
				turn_left(my_atoi(str_angle));
			}
			else
				AR_cmd_incorrecte();
		}

		else if (my_strcmp(commande,"S")){ //Stop
			AR_cmd_correcte();
			Stop();
		}

		else if (my_strcmp(commande,"G")) { //rejoit des coordonnee
			int xval, yval, angle;
			char i;
			for (i = 1; i < 4; i++) {
				char str_param[32] 	   = {0};
				char str_name_param[2] = {0};
				char str_val_param[6]  = {0};
				get_param(Pchaine_courante, i+1, str_param);
				get_complex_param(str_param, str_name_param, str_val_param);
				if (my_strcmp(str_name_param,"X")){
					xval = 10 * my_atoi(str_val_param);
					if (xval < -9900 || xval > 9900) {
						AR_cmd_incorrecte();
						return;
					}
				}
				else if (my_strcmp(str_name_param,"Y")){
					yval = 10 * my_atoi(str_val_param);
					if (yval < -9900 || yval > 9900) {
						AR_cmd_incorrecte();
						return;
					}
				}
				else if (my_strcmp(str_name_param,"A")){
					angle = my_atoi(str_val_param);
					if (xval < -180 || xval > 180) {
						AR_cmd_incorrecte();
						return;
					}
				}
			}
			go_coordinates_without_obstacles(xval,yval,angle);
		}

		else if (my_strcmp(commande,"TV")) {
			char str_param[4] = {0};
			get_param(Pchaine_courante,1,str_param);
			if (my_strlen(str_param) > 0){
				int param = my_atoi(str_param); 
				if (param > 6 && param < 101){
					AR_cmd_correcte();
					vitesse_par_default = param;
				}
				else 
					AR_cmd_incorrecte();
			}
			else 
				AR_cmd_incorrecte();
		}

		else {
			AR_cmd_incorrecte();
		}
	}
	else {
			AR_cmd_incorrecte();
	}
	RAZ_str(Pchaine_courante);
}

void Interrupt_Time(void) interrupt 16 {//interruption declancher par l'overflow du Timer 0 (toutes les us)
	T4CON &= ~(1<<7); //interrupt flag
    Time_increment();
}

/*
Interruption genere par INT6 permettant de mesurer le temps a l'etat haut du signal Echo
*/

void int6 () interrupt 18
{
	if ((P3IF & 0x04) == 0x04)
	{
		int* bool_echo1;
		bool_echo1 = get_bool_echo1_AV();
		TL2 = 0x00;
		TH2 = 0x00;
		P3IF |= 0x04;
		*bool_echo1 = 1;
		P3IF &= 0xBB;
	} else 
	{
		float* T;
		int* bool_echo2;
		commandCapture = 1;
		commandCapture = 0;
		T = get_T_AV();
		bool_echo2 = get_bool_echo2_AV();
		*T = RCAP2/22.1184;
		*bool_echo2 = 1;

		P3IF &= 0xBF;
		EXF2 = 0;
	}
}

/*
Interruprion overflow Timer2
Permet de rappeler la commande de detection d'obstacle si temps d'acquisition trop long
*/

void intT2 () interrupt 5
{
	int* bool_trig_AV;
	bool_trig_AV = get_bool_trig_AV();
	measureCycle += 1;
	if (measureCycle == 21) // 60.9 ms
	{
		*bool_trig_AV = 1;
		measureCycle = 1;
	}
	TF2 = 0;
}

/*
void Interrupt_UART0(void) interrupt 4 { //interruption declanch�e par l'UART0
    if (RI0 == 1){//si on vient de recevoir un caractere
			caractere_recu_fct_uart0(SBUF0);
      //caractere_recu = SBUF0;
    }
		else {
			Flag_TX0_fct();
		}
			
	  RI0 = 0; //Rx flag
		TI0 = 0;
		
}


void Interrupt_UART1(void) interrupt 20 { //interruption declanch�e par l'UART0
    if ((SCON1 & 0x01) == 0x01){//si on vient de recevoir un caractere
			caractere_recu_fct_uart1(SBUF1);
			Flag_RX1_fct();
    }
	else {
		Flag_TX1_fct();
	}
		SCON1 &= ~(1<<0);//Rx flag
		SCON1 &= ~(1<<1);//Tx flag
}
*/

















