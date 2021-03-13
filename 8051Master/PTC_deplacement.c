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
/*

void turn_right(int angle){
	//TODO
}

void turn_left(int angle){
	turn_right(-angle);
}


//variables globales pour cette fonction
char flag_calcule_angle = 1;
int x_robot = 0; //position actualle en cm
int y_robot = 0; //position actualle en cm
short teta_angle = 0;//angle des coordonnees cible


int go_coordinates(int coord_x, int coord_y){
	if (flag_calcule_angle == 1) 
		teta_angle = my_atan((coord_x - x_robot)/(coord_y - y_robot));
	else {
		if (can_go(10)){

		}
	}
	




}
*/

