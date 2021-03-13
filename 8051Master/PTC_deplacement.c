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
#include "PTC_UART.h" // pour Send_str_uart1
#include "PTC_math.h" //pour go_coordinates


#define AUTO_SPEED_FAST 30		  //vitesse de depplacement lorsque le robot rejoit automatiquement des coordonnees
#define AUTO_SPEED_SLOW 10
#define DISTANCE_DETECTION 10 //distance de detection des obstacle

void Avancer(char *str_vitesse){
    char data[64] = "mogo 1:";
	my_strcat(data,str_vitesse);
	my_strcat(data," 2:");
	my_strcat(data,str_vitesse);
	my_strcat(data,"\r");
	Send_str_uart1("\r"); //remet a 0 le serializer
	Send_str_uart1(data); //evoie du message
}

void Stop(void){
    Send_str_uart1("\r"); //remet a 0 le serializer
	Send_str_uart1("stop\r");
}


void turn_right(int angle){
	angle_robot -= angle; 
	//TODO
}

void turn_left(int angle){
	turn_right(-angle);
}


//variables globales pour cette fonction
char flag_calcule_angle = 1;
int x_robot = 0; //position actualle en cm
int y_robot = 0; //position actualle en cm
int angle_robot = 0;
short teta_angle_dest = 0;//angle des coordonnees cible



int go_coordinates(int coord_x, int coord_y){
	if (coord_x != x_robot || coord_y != y_robot){
		if (flag_calcule_angle == 1){
			teta_angle_dest = my_atan((coord_x - x_robot)/(coord_y - y_robot));
			if (teta_angle_dest - angle_robot != 0){ //aligne le robot dans la direction des coordonnee
				turn_right(teta_angle_dest - angle_robot);
			}
			flag_calcule_angle == 0;
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
}