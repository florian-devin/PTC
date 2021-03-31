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
#include "PTC_timer.h"
#include "UART1_RingBuffer_lib.h"
#include "UART0_RingBuffer_lib.h"
#include "PTC_math.h" //pour go_coordinates
#include "PTC_detection.h" //pour go_coordinate et bypass_obstacle
#include "PTC_convertion.h"
#include <math.h>

#define PI        3.141593

#define AUTO_SPEED_FAST 10		  //vitesse de depplacement lorsque le robot rejoit automatiquement des coordonnees
#define AUTO_SPEED_SLOW 5
#define DISTANCE_DETECTION 10 //distance de detection des obstacle
#define POURCENTAGE_ERREUR_POS_ROBOT 5 //en %
#define TEMP_ATTENTE_ROBOT_APRES_POS_ANGLE 4000 //en ms

//variables globales pour la fct go_coordinates
extern char state_go_coordinates;
extern int x_robot; //position actualle en cm
extern int y_robot; //position actualle en cm
extern int angle_robot; //angle actualle en deg

short teta_angle_dest = 0;//angle des coordonnees cible
unsigned int capture_wait_time;
char str_distance_go_coord[12] = {0}; //dans la fct go coord c'est la distance en nb de ticks que doit faire le robot en ligne droite

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

void Parcour_dist(char *str_distance_go_coord,char *str_vitesse){
	char chaine[64] = "digo 1:";
	my_strcat(chaine,str_distance_go_coord);
	my_strcat(chaine,":");
	my_strcat(chaine,str_vitesse);
	my_strcat(chaine," 2:");
	my_strcat(chaine,str_distance_go_coord);
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

void RAZ_encoder(char *Id) {
	char chaine[32] = "clrenc";
	my_strcat(chaine, Id);
	my_strcat(chaine, "\r");
	serOutstring_uart1(chaine); //evoie du message
	while (serInchar_uart1()==0); //attend un caractere de retour
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
	//if ((100*abs(coord_x - x_robot)/coord_x) > POURCENTAGE_ERREUR_POS_ROBOT && (100*abs(coord_y - y_robot)/coord_y) > POURCENTAGE_ERREUR_POS_ROBOT){
	if (state_go_coordinates == 1){ //positionnement de l'angle
		teta_angle_dest = (int)(180.0 * atan2((coord_x - x_robot),(coord_y - y_robot))/PI);
		turn_right(90 - teta_angle_dest);//aligne le robot dans la direction des coordonnee
		state_go_coordinates = 2;
		capture_wait_time = get_time_ms();
	}
	else if (state_go_coordinates == 2) {//attent pour que le robot ai le temp de rester en place
		if (get_time_ms() - capture_wait_time > TEMP_ATTENTE_ROBOT_APRES_POS_ANGLE) { //si les 4s sont passe
			state_go_coordinates = 3;
		}	
	}
	else if (state_go_coordinates == 3){ //demande de deplacement en ligne droite
		char str_vitesse[5] = {0};
		RAZ_str(str_distance_go_coord); 
		my_itoa(AUTO_SPEED_FAST,str_vitesse);
		my_itoa((int)(624*sqrt((coord_x - x_robot)*(coord_x - x_robot) + (coord_y - y_robot)*(coord_y - y_robot))/(PI*60)),str_distance_go_coord);
		RAZ_encoder("1");
		Parcour_dist(str_distance_go_coord, str_vitesse);
		state_go_coordinates = 4;
	}
	else if (state_go_coordinates == 4){  //on get si on est arrive
		if (get_encoder("1") > str_distance_go_coord - 10) // 10 est l'erreur sur les ticks
			state_go_coordinates = 5;
	}

	else if  (state_go_coordinates == 5){//changer l'angle final
		turn_right(angle - (90 - teta_angle_dest));//placement en pas final
		state_go_coordinates = 6;
		capture_wait_time = get_time_ms();
	}

	else if (state_go_coordinates == 6) {
		if (get_time_ms() - capture_wait_time > TEMP_ATTENTE_ROBOT_APRES_POS_ANGLE) //si les 4s sont passe
			state_go_coordinates = 7;	
	}
	else if (state_go_coordinates == 7) {//envoie de B au pc
		serOutchar('B');//arrive a la fin
		state_go_coordinates = 0;
	}
}
