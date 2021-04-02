//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_pointeurLumineux.h
// Destination: 8150F020 Carte Slave
// Description: Fonctions d'allumage du pointeur lumineux
//------------------------------------------------------



//Allume le pointeur lunineux en clignotant
//@param Intensite intensite de la lumiere (signal PWM)
//@param Lum_ON Temps durant lequelle la lumiere reste alume en ms
//@param Lum_OFF Temps durant la led est eteinte en ms
//@param Lum_Nbre Nomble de cycle d'allumage 
void Lumiere (const unsigned char Intensite, const unsigned int Lum_ON, const unsigned int Lum_OFF, const unsigned char Lum_Nbre);

//Extinction inconditionnelle  de la source lumineuse
void Lumiere_Stop (void);


//Fonction de boucle de Lumiere_loop doit etre apele dans le programme principale si Lumiere_loop_Enable = 1
//@param *Intensite adresse de intensite de la lumiere (signal PWM)
//@param *Lum_ON adresse de Temps durant lequelle la lumiere reste alume en ms
//@param *Lum_OFF adresse de Temps durant la led est eteinte en ms
//@param *Lum_Nbre adresse de Nomble de cycle d'allumage (decrementé au fils du temps)
void Lumiere_loop (const unsigned char Intensite, const unsigned int Lum_ON, const unsigned int Lum_OFF, unsigned char *Lum_Nbre);