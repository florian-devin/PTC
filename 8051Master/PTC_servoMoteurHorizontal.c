//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Ivana YEMELE , Xabi FAUT 
// Email: ivana.yemele-tsafack@cpe.fr ; xabi.faut@cpe.fr
// Nom: PTC.servoMoteurHorizontal.c
// Destination: 8150F020 Carte Master
// Description: déplacement Horizontal du servo moteur 
//------------------------------------------------------
#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "PTC_servoMoteurHorizontal.h"
#include <math.h>

// variables
char  Angle_actuel = 90; // Angle actuel du servomoteur
unsigned char time_move; //temps mis pour se deplacer 

// Prototypes de Fonctions

// Cette fonction calcule et retourne le temps en fonction de Angle donne en argument
// Penser a adapter au servo moteur
unsigned char Duree(char Angle) {
	time_move=(abs(Angle-Angle_actuel)*22)/60 ; //temps en centieme de secondes HS-485HB
	return time_move;
}
// Cette fonction prend Angle en argument et modifie les valeurs des PCA0CPH0.
void rechargement(char Angle){
	unsigned int PCA0CP0_inter = 65535.0-((55.0*(Angle+90.0))+10000.0)*(65535.0/355600.0);
	PCA0CPL0 = PCA0CP0_inter; 
	PCA0CPH0 = PCA0CP0_inter >> 8; 
}

//Pour la generation du signal correspondant on bon duty et du temps de mouvement
unsigned char CDE_Servo_H(char Angle) {
	time_move = Duree(Angle);
	rechargement(Angle);
	Angle_actuel = Angle;
	return time_move;
}