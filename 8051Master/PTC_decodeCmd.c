//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.c
// Destination: 8150F020 Carte Master
// Description: Fonctions qui permette de cecomposer les commandes du PC de commande
//------------------------------------------------------
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_deplacement.h"
#include "PTC_geter_cmd.h"
#include "PTC_servoMoteurHorizontal.h"
#include "PTC_strOperateurs.h"
#include "PTC_telemetre.h"
#include "UART0_RingBuffer_lib.h"
#include "SPI_RingBuffer_Master.h"
//------------------------------------------------------
//------------------------------------Variables Globales
extern int              epreuve_enable;
extern int              vitesse_par_default;

extern int              go_coordinates_x;
extern int              go_coordinates_y;
extern int              go_coordinates_angle;
extern char             state_go_coordinates;

extern unsigned char    temp_servo_H;
extern char             flag_print_arrive_servo_H;
extern char 			Angle_actuel;

//------------------------------------------------------
//------------------------------------------------------

void Cmd_epreuve_D(const char *Pchaine_courante) {
    char str_type_epreuve[2] = {0};

	get_param(Pchaine_courante, 1, str_type_epreuve);
	if (my_strlen(str_type_epreuve) > 0) {//il y a un parametre
		int type_epreuve = my_atoi(str_type_epreuve);
		if (type_epreuve > 0 && type_epreuve < 9) {
			AR_cmd_correcte();
			epreuve_enable = type_epreuve;
			serOutstring("I Le robot a demarre l'epreuve !\r\n");
		}
		else
			AR_cmd_incorrecte();
	}
	else {
		AR_cmd_correcte();
		epreuve_enable = 1;
		serOutstring("I Le robot a demarre l'epreuve !\r\n");
	}
}

void Cmd_epreuve_E(const char *Pchaine_courante) {
    char str_type_epreuve[2] = {0};

	get_param(Pchaine_courante, 1, str_type_epreuve);
	AR_cmd_correcte(); 
	epreuve_enable = 0;
}

void Cmd_epreuve_A(const char *Pchaine_courante) {
    char str_param[12]  = {0};
	int param           = 0  ;

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

void Cmd_epreuve_B(const char *Pchaine_courante) {
    char str_param[12]  = {0};
	int param           = 0  ;

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

void Cmd_epreuve_RG(void) {
    AR_cmd_correcte();
	turn_left(90);
} 

void Cmd_epreuve_RD(void) {
    AR_cmd_correcte();
	turn_right(90);
}

void Cmd_epreuve_RC(const char *Pchaine_courante) {
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

void Cmd_epreuve_RA(const char *Pchaine_courante) {
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

void Cmd_epreuve_S(void) {
    AR_cmd_correcte();
	Stop();
}

void Cmd_epreuve_G(const char *Pchaine_courante) {
	int xval, yval, angle;
	char i;

	for (i = 0; i < 3; i++) {
		char str_param[32] 	   = {0};
		char str_name_param[2] = {0};
		char str_val_param[6]  = {0};

		get_param(Pchaine_courante, i+1, str_param);
		get_complex_param(str_param, str_name_param, str_val_param);
		if (my_strcmp(str_name_param,"X")){
			xval = 10 * my_atoi(str_val_param);
			if (xval < -9900 || xval > 9900) {
				AR_cmd_incorrecte();
				return; //on sort de la fonction
			}
		}
		else if (my_strcmp(str_name_param,"Y")){
			yval = 10 * my_atoi(str_val_param);
			if (yval < -9900 || yval > 9900) {
				AR_cmd_incorrecte();
				return; //on sort de la fonction
			}
		}
		else if (my_strcmp(str_name_param,"A")){
			angle = my_atoi(str_val_param);
			if (angle < -180 || angle > 180) {
				AR_cmd_incorrecte();
				return; //on sort de la fonction
			}
		}
		else 
			AR_cmd_correcte();		
	}
	state_go_coordinates = 1; //activation de la machine d'etat pour rejoindre les ccords
	go_coordinates_x     = xval;
	go_coordinates_y     = yval;
	go_coordinates_angle = angle;
}

void Cmd_epreuve_TV(const char *Pchaine_courante) {
    char str_param[4] = {0};

	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0){
		int param = my_atoi(str_param); 

		if (param > 6 && param < 101) {
			AR_cmd_correcte();
			vitesse_par_default = param;
		}
		else 
			AR_cmd_incorrecte();
	}
	else 
		AR_cmd_incorrecte();
}

void Cmd_epreuve_CS(const char *Pchaine_courante) {
    char str_param[2] = {0};

	get_param(Pchaine_courante,1,str_param);
	if (my_strcmp(str_param, "V")) { //cervo vertical
		AR_cmd_correcte();
		my_strcat(Pchaine_courante,"\r");
		serOutstring_SPI(Pchaine_courante);
	}

	else if (str_param[0] == 'A') { //cervo par default H
		char str_param_name[2]  = {0};
		char str_param_value[4] = {0};
		get_complex_param(str_param, str_param_name, str_param_value);
		if (my_strcmp(str_param_name, "A")) {
			char angle = (char)my_atoi(str_param_value);
			if (angle > -91 && angle < 91) {
				AR_cmd_correcte();
				temp_servo_H = (char)10*CDE_Servo_H(angle);
				flag_print_arrive_servo_H =1;
			}
			else
				AR_cmd_incorrecte();
		}
		else
			AR_cmd_incorrecte();
	}

	else if (my_strcmp(str_param,"H")) {//cervo horizontal 
		char str_param2[8] = {0};
		get_param(Pchaine_courante, 2,str_param2);
		if (my_strlen(str_param2) > 0) {
			char str_param_name[2]  = {0};
			char str_param_value[4] = {0};
			get_complex_param(str_param2, str_param_name, str_param_value);
			if (my_strcmp(str_param_name, "A")) {
				char angle = (char)my_atoi(str_param_value);
				if (angle > -91 && angle < 91) {
					AR_cmd_correcte();
					temp_servo_H = (char)10*CDE_Servo_H(angle);
					flag_print_arrive_servo_H =1;
				}
				else
					AR_cmd_incorrecte();
			}
			else
				AR_cmd_incorrecte();
		}
		else {
			AR_cmd_correcte();
			temp_servo_H = (char)10*CDE_Servo_H(0);
			flag_print_arrive_servo_H =1;
		}
	}
}

void Cmd_epreuve_L(const char *Pchaine_courante) { //Commande pour la carte esclave
	char str_param[8] = {0};
	char num_param = 1;
	do {
		get_param(Pchaine_courante,num_param,str_param);
		if (my_strlen(str_param) != 0){
			char str_value[4] = {0};
			char str_name[2]  = {0};
			num_param++;
			get_complex_param(str_param,str_name,str_value);
			if (my_strcmp(str_name, "I")){
				char value = my_atoi(str_value);
				if (!(value > 0 && value < 101)) {
					AR_cmd_incorrecte();
					return;
				}
			}
			else if  (my_strcmp(str_name, "D") || my_strcmp(str_name, "E") || my_strcmp(str_name, "N")) {
				char value = my_atoi(str_value);
				if (!(value >= 0 && value < 100)) {
					AR_cmd_incorrecte();
					return;
				}
			}
			else {
				AR_cmd_incorrecte();
				return;
			}
		} 
	} while (my_strlen(str_param) != 0);
	
	AR_cmd_correcte();
	my_strcat(Pchaine_courante,"\r");
	serOutstring_SPI(Pchaine_courante);
}

void Cmd_epreuve_LS(const char *Pchaine_courante) {//Commande pour la carte esclave
	char str_param[2] = {0};
	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0)
		AR_cmd_incorrecte();
	else {
		AR_cmd_correcte();
		my_strcat(Pchaine_courante,"\r");
		serOutstring_SPI(Pchaine_courante);
	}
}

void Cmd_epreuve_MI(const char *Pchaine_courante) {
	char str_param[2] = {0};
	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0) {
		AR_cmd_incorrecte();
	}
	else {
		AR_cmd_correcte();
		//TODO : Ajouter la fonction de meusure du courrant instantane
	}
}

void Cmd_epreuve_ME(const char *Pchaine_courante) {
	char str_param[2] = {0};
	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0) {
		AR_cmd_incorrecte();
	}
	else {
		AR_cmd_correcte();
		//TODO : Envoyer la consomation totale du robot (depuis le debut de l'epreuve)
	}
}

void Cmd_epreuve_MOU(const char *Pchaine_courante) {
	char str_param[2] = {0};
	float dist_AV = -2.0;
	float dist_AR = -2.0;
	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0 && my_strcmp(str_param,"D")) {
		dist_AV = MES_Dist_AV();
		dist_AR = MES_Dist_AR();
		AR_cmd_correcte();
	}
	else if(my_strlen(str_param) == 0){
		dist_AV = MES_Dist_AV();
		AR_cmd_correcte();
	}
	else 
		AR_cmd_incorrecte();
	if (dist_AV != -2.0) {
		char msg[64] 	  = "KOB ";
		char strinter[10] = {0};
		char str_angle[5] = {0};
		char str_dist[5]  = {0}; 
		//int  int_angle = 0;
		//int  int 
		if (dist_AV == -1.0)
			dist_AV = 0.0;
		if (dist_AR == -1.0)
			dist_AR = 0.0;
		my_itoa((int)Angle_actuel,str_angle);
		my_itoa((int)dist_AV, str_dist);
		my_strcat(msg,str_angle);
		my_strcat(msg,":");
		my_strcat(msg,str_dist);
		if (dist_AR != -2.0){
			unsigned char angle_AR = 0;
			if (Angle_actuel >= 0)
				angle_AR -= 180;
			else 
				angle_AR += 180;
		
			my_strcat(msg," ");
			my_strcat(msg, my_itoa((int)angle_AR,strinter));
			my_strcat(msg,":");
			my_strcat(msg,my_itoa((int)dist_AR,strinter));
		}
		serOutstring(msg);
	}
	
}

void Cmd_epreuve_MOB(const char *Pchaine_courante) {
	char str_param[2] = {0};
	char num_param = 1;
	do {
		get_param(Pchaine_courante,num_param++,str_param);
		if (my_strcmp(str_param,"D")) {
			//TODO : detection angulaire de 180 deg
		}
		else if (my_strlen(str_param) > 0) {
			char str_name[2] = {0};
			char str_value[3] = {0};
			get_complex_param(str_param,str_name,str_value);
			if (my_strcmp(str_name,"A")){
				char angle = my_atoi(str_value);
				if (angle <= 45 && angle >= 5 && angle%5 == 0){
					//TODO : detection avec un pas de angle
					//TODO : a faire avec des variables et appeler qu'une seul fois la fonction
				}
				else {
					AR_cmd_incorrecte();
					return;
				}
			}
		}
	} while (my_strlen(str_param) != 0);
}

void Cmd_epreuve_MOS(const char *Pchaine_courante) {
	char str_param[2] = {0};
	char num_param = 1;
	do {
		get_param(Pchaine_courante,num_param++,str_param);
		if (my_strcmp(str_param,"D")) {
			//TODO : detection angulaire de 180 deg
		}
		else if (my_strlen(str_param) > 0) {
			char str_name[2] = {0};
			char str_value[3] = {0};
			get_complex_param(str_param,str_name,str_value);
			if (my_strcmp(str_name,"A")){
				char angle = my_atoi(str_value);
				if (angle <= 45 && angle >= 5 && angle%5 == 0){
					//TODO : detection avec un pas de angle
					//TODO : a faire avec des variables et appeler qu'une seul fois la fonction
				}
				else {
					AR_cmd_incorrecte();
					return;
				}
			}
		}
	} while (my_strlen(str_param) != 0);
}

void Cmd_epreuve_ASS(const char *Pchaine_courante) {
	char str_param[3] = {0};
	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) != 0){
		char duree = my_atoi(str_param);
		if (duree > 0 && duree < 100) {
			AR_cmd_correcte();
			//TODO transmetre a la carte slave
		}
		else {
			AR_cmd_incorrecte();
			return;
		}
	}
	else {
		AR_cmd_correcte();
		//TODO transmetre a la carte slave avec la duree par default 
	}
}

void Cmd_epreuve_SD(const char *Pchaine_courante) {
	char str_param[2] = {0};
	char num_param = 1;
	do {
		get_param(Pchaine_courante,num_param,str_param);
		if (my_strlen(str_param) != 0){
			char str_value[4] = {0};
			char str_name[2]  = {0};
			num_param++;
			get_complex_param(str_param,str_name,str_value);
			if  (my_strcmp(str_name, "F") || my_strcmp(str_name, "P") || my_strcmp(str_name, "W") || my_strcmp(str_name, "B")) {
				char value = my_atoi(str_value);
				if (!(value >= 0 && value < 100)) {
					AR_cmd_incorrecte();
					return;
				}
			}
			else {
				AR_cmd_incorrecte();
				return;
			}
		} 
	} while (my_strlen(str_param) != 0);
	
	if (num_param < 2) {//parametre par default
		//TODO : mettre des paramettre par default avant d'envoyer a l'esclave
		AR_cmd_correcte();
		my_strcat(Pchaine_courante,"\r");
		serOutstring_SPI(Pchaine_courante);
	}
	else {
		AR_cmd_correcte();
		my_strcat(Pchaine_courante,"\r");
		serOutstring_SPI(Pchaine_courante);
	}
}

void Cmd_epreuve_PPH(const char *Pchaine_courante){
	char str_param[8] = {0};
	char num_param = 1;
	do {
		get_param(Pchaine_courante,num_param,str_param);
		if (my_strlen(str_param) != 0){
			if (!(str_param[0] == 'O' || str_param[0] == 'S' || str_param[0] == 'C'|| str_param[0] == 'E' || str_param[0] == 'N') && my_strlen(str_param) < 2){
				AR_cmd_incorrecte();
				return;
			}
			else if (str_param[0] == 'E'){
				char str_name[2]  = {0};
				char str_value[4] = {0};
				int  value        = 0;
				get_complex_param(str_param,str_name,str_value);
				value = my_atoi(str_value);
				if (value < 0 && value > 99){
					AR_cmd_incorrecte();
					return;
				}
			}
			else if (str_param[0] == 'N'){
				char str_name[2]  = {0};
				char str_value[4] = {0};
				int  value        = 0;
				get_complex_param(str_param,str_name,str_value);
				value = my_atoi(str_value);
				if (value < 1 && value > 255){
					AR_cmd_incorrecte();
					return;
				}
			}
		}
	} while (my_strlen(str_param) != 0);
	AR_cmd_correcte();
	my_strcat(Pchaine_courante,"\r");
	serOutstring_SPI(Pchaine_courante);
}

void Cmd_epreuve_SPH(const char *Pchaine_courante){
	char str_param[2] = {0};
	get_param(Pchaine_courante,1,str_param);
	if (my_strlen(str_param) > 0)
		AR_cmd_incorrecte();
	else {
		AR_cmd_correcte();
		my_strcat(Pchaine_courante,"\r");
		serOutstring_SPI(Pchaine_courante);
	}
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Commandes recu de l'esclave SPI


void Cmd_epreuve_SPICSV(void) {//Cette commande signifie que le servomoteur verticale est ariive en position
	serOutstring("AS V");
}









