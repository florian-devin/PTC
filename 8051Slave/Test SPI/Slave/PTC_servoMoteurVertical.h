//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_servoMoteurVertical.h
// Destination: 8150F020 Carte Slave
// Description: Fonctions de commande du cervo-moteur vertical
//------------------------------------------------------

//calcule le temps de deplacement du cervo 
//@param Angle angle de destination
//@return le temps estimé en centieme de secondes
unsigned char Time_deplacement_cervoV(char Angle);


//Modifie la position du cervo moteur 
//@param Angle angle de destination
//@return le temps estimé en centieme de secondes
unsigned char CDE_Servo_V(char Angle);
