//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Ivana YEMELE , Xabi FAUT 
// Email: ivana.yemele-tsafack@cpe.fr ; xabi.faut@cpe.fr
// Nom: PTC_servoMoteurHorizontal.h
// Destination: 8150F020 Carte Master
// Description: déplacement Horizontal du servo moteur 
//------------------------------------------------------

unsigned char Duree(char Angle) ;
void rechargement(char Angle) ; 

//Commande du cervo horizonale 
//@param Angle angle souhaite en deg
//@return Le temp que le servo va mettre pour rejoindre sa position. en centieme de seconde
unsigned char CDE_Servo_H(char Angle) ;

