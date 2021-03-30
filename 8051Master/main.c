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



//-----------------------------------------------------------------------------
//variables globales
//En lien avec l'UART0
//char interrupt_out = 0;        //mon flag pour le caractere recu
//char caractere_recu = '\0';    //caractere qui vient d'etre recu
char chaine_courante[64] = {0};//chaine total qui vas contenir le mot recu (20 caractere max)

char i = 0;

//En lien avec le temp
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//headers
//void main();
void setup();
void startup();
void loop();
void decodage_commande(char *Pchaine_courante);

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
		my_println("Rx_chaine = 1");
		decodage_commande(chaine_courante);
	}

}

void decodage_commande(char *Pchaine_courante){ //fonction qui decode les commades et les applique
	char commande[7] = {0};
	my_print("La chaine est :");
	my_println(Pchaine_courante);
	get_commande(Pchaine_courante,commande);
	my_print("La commande est :");
	my_println(commande);

	if (my_strcmp(commande,"A")){ //Avancer
		char str_param[12] = {0};
		int param = 0;
		my_println("Avancer");
		get_param(Pchaine_courante,1, str_param);
		my_print("La vitesse est :");
		my_println(str_param);
		param = my_atoi(str_param);
		if (param <= 100 && param >= 5){
			my_println("Vitesse valide !");
			AR_cmd_correcte();
			Avancer(str_param);
			my_println("message transmit avec AR !");
		}
		else {
			AR_cmd_incorrecte();
		}
	}

	else if (my_strcmp(commande,"B")){//Reculer
		char str_param[12] = {0};
		int param = 0;
		my_println("Reculer");
		get_param(Pchaine_courante,1, str_param);
		my_print("La vitesse est :");
		my_println(str_param);
		param = my_atoi(str_param);
		if (param <= 100 && param >= 5){
			my_println("Vitesse valide !");
			AR_cmd_correcte();
			Reculer(str_param);
			my_println("message transmit avec AR !");
		}
	}

	else if (my_strcmp(commande,"RG")){
		my_println("Tourner a gauche");
		AR_cmd_correcte();
		turn_left(90);
		my_println("message transmit avec AR !");
	}

	else if (my_strcmp(commande,"RD")){
		my_println("Tourner a droite");
		AR_cmd_correcte();
		turn_right(90);
		my_println("message transmit avec AR !");
	}

	else if (my_strcmp(commande,"RC")){
		char str_param[12] = {0};
		my_println("Rotation complete");
		get_param(Pchaine_courante,1, str_param);
		my_print("Le sens de rotation est :");
		my_println(str_param);
		if (my_strcmp(str_param,"D")){
			my_println("Rotation droite");
			AR_cmd_correcte();
			turn_right(180);
			my_println("message transmit avec AR !");
		}
		else if (my_strcmp(str_param,"G")){
			my_println("Rotation gauche");
			AR_cmd_correcte();
			turn_left(180);
			my_println("message transmit avec AR !");
		}
		else 
			AR_cmd_incorrecte();
	}

	else if (my_strcmp(commande,"RA")){
		char str_param[8] = {0};
		char str_angle[4] = {0};
		char direction[2] = {0};
		my_println("Rotation d'un angle");
		get_param(Pchaine_courante,1,str_param);
		get_complex_param(str_param,direction,str_angle);
		if (my_strcmp(direction,"D")){
			my_println("Rotation droite");
			AR_cmd_correcte();
			turn_right(my_atoi(str_angle));
			my_println("message transmit avec AR !");
		}
		else if (my_strcmp(direction,"G")){
		    my_println("Rotation gauche");
			AR_cmd_correcte();
			turn_left(my_atoi(str_angle));
			my_println("message transmit avec AR !");
		}
		else
			AR_cmd_incorrecte();
	}

	else if (my_strcmp(commande,"S")){ //Stop
		AR_cmd_correcte();
		Stop();
		my_println("message transmit avec AR !");
	}

	else if (my_strcmp(commande,"G")) {
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

	else {
		AR_cmd_incorrecte();
	}
	
	RAZ_str(Pchaine_courante);
}
void Interrupt_Time(void) interrupt 1 {//interruption declancher par l'overflow du Timer 0 (toutes les us)
    TF0 = 0; //interrupt flag
    Time_increment();
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















