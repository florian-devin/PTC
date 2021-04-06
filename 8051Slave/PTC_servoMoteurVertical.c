//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_servoMoteurVertical.c
// Destination: 8150F020 Carte Slave
// Description: Fonctions de commande du cervo-moteur vertical
//------------------------------------------------------
#include "PTC_PWM.h"
#include "PTC_servoMoteurVertical.h"

// variables
char  Angle_actuel = 90; // Angle actuel du servomoteur


// Cette fonction calcule et retourne le temps en fonction de Angle donne en argument
// Penser a adapter au servo moteur
unsigned char Time_deplacement_cervoV(char Angle) {
	return (abs(Angle-Angle_actuel) * 22 / 60); //temps en centieme de secondes HS-485HB
}



unsigned char CDE_Servo_V(char Angle) {
    analogWrite_CEX1(Angle); //TODO calibrer ce mouvement  
	Angle_actuel = Angle;
	return Time_deplacement_cervoV(Angle);
}
