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

//variables globales pour la fct go_coordinates
char flag_calcule_angle = 1;
int x_robot = 0; //position actualle en cm
int y_robot = 0; //position actualle en cm
int angle_robot = 0;
short teta_angle_dest = 0;//angle des coordonnees cible

int angle_initial = 0; //l'angle est remi a zero a chanque appelle de go_coordinates_without_obstacles et il permet a la fct de positionner le robot sur un certain angle a la fin du deplacement


void Avancer(char *str_vitesse){
    char chaine[64] = "mogo 1:";
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine," 2:");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine,"\r");
	serOutstring_uart1(chaine); //evoie du message
	Wait_Accuse_RX_Robot();     //attend l'accuse de reception du robot
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
	Wait_Accuse_RX_Robot();     //attend l'accuse de reception du robot
}

void Reculer(char *str_vitesse){
    char chaine[32] = "mogo 1:-";
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine," 2:-");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine,"\r");
	serOutstring_uart1(chaine); //evoie du message
	Wait_Accuse_RX_Robot();     //attend l'accuse de reception du robot
}

void Stop(void){
	serOutstring_uart1("stop\r");
	Wait_Accuse_RX_Robot();
}

int get_encoder(char *Id){
	char chaine[32] = "getenc";
	my_strcat(chaine, Id);
	my_strcat(chaine, "\r");
	serOutstring_uart1(chaine); //evoie du message
	return 0;
}


void turn_right(int angle){
	int ticks       = angle*1165/90;
	char chaine[32] = "digo 1:";
	char str[8] = {0};
	my_strcat(chaine,my_itoa(ticks,str));
	my_strcat(chaine,":25 2:");
	my_strcat(chaine,my_itoa(-ticks,str));
	my_strcat(chaine,":25\r");
	Wait_Accuse_RX_Robot();
	angle_robot -= angle; 
}

void turn_left(int angle){
	turn_right(-angle);
}



void go_coordinates_without_obstacles(int coord_x, int coord_y, int angle){
	angle_initial = 0;
	if (coord_x != x_robot && coord_y != y_robot){
		if (flag_calcule_angle == 1){
			teta_angle_dest = (int)(180.0 * atan2((coord_x - x_robot),(coord_y - y_robot))/PI);
			if (teta_angle_dest - angle_robot != 0){ //aligne le robot dans la direction des coordonnee
				turn_right(teta_angle_dest - angle_robot);
				angle_initial -= teta_angle_dest - angle_robot;
			}
			flag_calcule_angle = 0;
		} 
		if (flag_calcule_angle == 0){
			char str_vitesse[5] = {0};
			char str_distance[10] = {0};
			my_itoa(AUTO_SPEED_FAST,str_vitesse);
			my_itoa((int)(624*sqrt((coord_x - x_robot)*(coord_x - x_robot) + (coord_y - y_robot)*(coord_y - y_robot))/(PI*60)),str_distance);
			Parcour_dist(str_distance,str_vitesse);
			x_robot = coord_x;
			y_robot = coord_y;
			if (angle != angle_initial) {
				turn_right(angle - angle_initial);
			}
			flag_calcule_angle = 1;
		}
	}
}

/*
int go_coordinates(int coord_x, int coord_y){
	if (coord_x != x_robot || coord_y != y_robot){
		if (flag_calcule_angle == 1){
			teta_angle_dest = my_atan((coord_x - x_robot)/(coord_y - y_robot));
			if (teta_angle_dest - angle_robot != 0){ //aligne le robot dans la direction des coordonnee
				turn_right(teta_angle_dest - angle_robot);
			}
			flag_calcule_angle = 0;
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
		flag_calcule_angle = 1;
	}
	else if (can_go_left(DISTANCE_DETECTION)){
		turn_left(90);
		while (!can_go_right(DISTANCE_DETECTION))
			Avancer(AUTO_SPEED_SLOW);
		Stop();
		flag_calcule_angle = 1;
	}
}*/