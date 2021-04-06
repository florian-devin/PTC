//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_PWM.h
// Destination: 8150F020 Carte Slave
// Description: Fonctions de generation de signaux pwm
//------------------------------------------------------

//Set le PWM 8bits CEX0 (ici le pointeur lumineux)
//@param value rapport cyclique de 0 a 255
void analogWrite_CEX0(const unsigned char value);

//Set le PWM 8bits CEX1 (ici le cervo moteur verticale)
//@param value rapport cyclique de 0 a 255
void analogWrite_CEX0(const unsigned char value);