//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_deplacement.c
// Destination: 8150F020 Carte Master
// Description: Fonctions de deplacement elementaires 
//------------------------------------------------------

#include "PTC_deplacement.h"
#include "PTC_strOperateurs.h"  // pour my_strcat
#include "PTC_UART.h" // 
#include "UART1_RingBuffer_lib.h"
#include "UART0_RingBuffer_lib.h"
#include "PTC_math.h" //pour go_coordinates
#include "PTC_detection.h" //pour go_coordinate et bypass_obstacle
#include "PTC_convertion.h"
#include <math.h>

#define PI        3.141593

#define AUTO_SPEED_FAST 30		  //vitesse de depplacement lorsque le robot rejoit automatiquement des coordonnees
#define AUTO_SPEED_SLOW 10
#define DISTANCE_DETECTION 10 //distance de detection des obstacle
#define POURCENTAGE_ERREUR_POS_ROBOT 5 //en %

//variables globales pour la fct go_coordinates
extern char flag_go_coordinates;
extern int x_robot; //position actualle en cm
extern int y_robot; //position actualle en cm
extern int angle_robot; //angle actualle en deg
int angle_glob, coord_x_glob, coord_y_glob;
short teta_angle_dest = 0;//angle des coordonnees cible

int angle_initial = 0; //l'angle est remi a zero a chanque appelle de go_coordinates_without_obstacles et il permet a la fct de positionner le robot sur un certain angle a la fin du deplacement


void Avancer(char *str_vitesse){
    char chaine[64] = "mogo 1:";
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine," 2:");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine,"\r");
	serOutstring_uart1(chaine); //evoie du message
	//Wait_Accuse_RX_Robot();     //attend l'accuse de reception du robot
	while (serInchar_uart1()==0); //attend un caractere de retour
}

void Parcour_dist(char *str_distance,char *str_vitesse){
	char chaine[64] = "digo 1:";
	my_strcat(chaine,str_distance);
	my_strcat(chaine,":");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine," 2:");
	my_strcat(chaine,str_distance);
	my_strcat(chaine,":");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine,"\r");
	serOutstring_uart1(chaine); //evoie du message
	//Wait_Accuse_RX_Robot();     //attend l'accuse de reception du robot
	while (serInchar_uart1()==0); //attend un caractere de retour
}

void Reculer(char *str_vitesse){
    char chaine[32] = "mogo 1:-";
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine," 2:-");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine,"\r");
	serOutstring_uart1(chaine); //evoie du message
	//Wait_Accuse_RX_Robot();     //attend l'accuse de reception du robot
	while (serInchar_uart1()==0); //attend un caractere de retour
}

void Stop(void){
	serOutstring_uart1("stop\r");
	while (serInchar_uart1()==0); //attend un caractere de retour
	//Wait_Accuse_RX_Robot();
}

long get_encoder(char *Id){
	char chaine[32] = "getenc";
	char reponse[16] = {0};
	char c,i = 0;
	char len_reponse = 0;
	my_strcat(chaine, Id);
	my_strcat(chaine, "\r");
	serOutstring_uart1(chaine); //evoie du message
	while ((c=serInchar_uart1())!='>'){
		reponse[i] = c;
		i++;
	}
	len_reponse = (char)my_strlen(reponse);
	reponse[len_reponse-1] = '\0'; //suppresion de '>'
	reponse[len_reponse-2] = '\0'; //suppresion de '\r'
	return((long)my_atoi((int)reponse));
}


void turn_right(int angle){
	long ticks      = (angle*(long)1165)/(long)90;
	char chaine[32] = "digo 1:";
	char str[12] = {0};
	my_strcat(chaine,my_itoa(ticks,str));
	my_strcat(chaine,":25 2:");
	my_strcat(chaine,my_itoa(-ticks,str));
	my_strcat(chaine,":25\r");
	serOutstring_uart1(chaine); //evoie du message
	//Wait_Accuse_RX_Robot();
	while (serInchar_uart1()==0); //attend un caractere de retour
	angle_robot -= angle; 
}

void turn_left(int angle){
	turn_right(-angle);
}



void go_coordinates_without_obstacles(int coord_x, int coord_y, int angle){
	if ((100*abs(coord_x - x_robot)/coord_x) > POURCENTAGE_ERREUR_POS_ROBOT && (100*abs(coord_y - y_robot)/coord_y) > POURCENTAGE_ERREUR_POS_ROBOT){
		if (flag_go_coordinates == 1){
			coord_x_glob = coord_x;
			coord_y_glob = coord_y;
			angle_glob   = angle;
			angle_initial = 0;
			teta_angle_dest = (int)(180.0 * atan2((coord_x - x_robot),(coord_y - y_robot))/PI);
			turn_right(teta_angle_dest - angle_robot);//aligne le robot dans la direction des coordonnee
			angle_initial -= teta_angle_dest - angle_robot;
			flag_go_coordinates = 0;
		}
		if (flag_go_coordinates == 0){
			char str_vitesse[5] = {0};
			char str_distance[10] = {0};
			my_itoa(AUTO_SPEED_FAST,str_vitesse);
			my_itoa((int)(624*sqrt((coord_x - x_robot)*(coord_x - x_robot) + (coord_y - y_robot)*(coord_y - y_robot))/(PI*60)),str_distance);
			Parcour_dist(str_distance,str_vitesse);
			if (angle != angle_initial) {
				turn_right(angle - angle_initial);
			}
			flag_go_coordinates = 1;
		}
	}
	else {//changer l'angle (on a attient les coords)
		turn_right(angle - angle_initial);
		flag_go_coordinates = 0; //raz du flag 
		serOutchar('B');//arrive a la fin
	}
}

/*
int go_coordinates(int coord_x, int coord_y){
	if (coord_x != x_robot || coord_y != y_robot){
		if (flag_go_coordinates == 1){
			teta_angle_dest = my_atan((coord_x - x_robot)/(coord_y - y_robot));
			if (teta_angle_dest - angle_robot != 0){ //aligne le robot dans la direction des coordonnee
				turn_right(teta_angle_dest - angle_robot);
			}
			flag_go_coordinates = 0;
		} 
		else {
			if (can_go(DISTANCE_DETECTION))
				Avancer(AUTO_SPEED_FAST);
			else{
				bypass_obstacle();
			}
		}
		return 0;
	}
	else{ //si le robot est arrive a destination
		Stop();
		return 1;
	}
}

//contourne l'obstacle (fonction bloquante a ameliorer) //TODO : rendre cette fonction plus blocante
//@param void
//@return void
void bypass_obstacle(){
	if(can_go_right(DISTANCE_DETECTION)){
		turn_right(90);
		while (!can_go_left(DISTANCE_DETECTION))
			Avancer(AUTO_SPEED_SLOW);
		Stop();
		flag_go_coordinates = 1;
	}
	else if (can_go_left(DISTANCE_DETECTION)){
		turn_left(90);
		while (!can_go_right(DISTANCE_DETECTION))
			Avancer(AUTO_SPEED_SLOW);
		Stop();
		flag_go_coordinates = 1;
	}
}*/