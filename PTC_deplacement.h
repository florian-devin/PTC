//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_deplacement.h
// Destination: 8150F020 Carte Master
// Description: Fonctions de deplacement elementaires 
//------------------------------------------------------

//Demmande au serializer d'avancer a la vitesse str_vitesse
//@param vitesse sous forme d'un char *
//@return rien
void Avancer(char *str_vitesse);

//Demmande au serializer darreter les moteurs
//@param rien
//@return rien
void Stop(void);