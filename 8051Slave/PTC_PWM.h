//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_PWM.h
// Destination: 8150F020 Carte Slave
// Description: Fonctions de generation de signaux pwm
//------------------------------------------------------

//Genere un signal PWM sur CEX0 avec un duty de value (la max est 255) le min est 0
//@param value valeur du duty cycle (0 - 255)
void analogWrite_CEX0(const unsigned char value);

