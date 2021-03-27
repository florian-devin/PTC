//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_convertion.h
// Destination: 8150F020 Carte Master
// Description: Fonctions de convertion
//------------------------------------------------------

//Converti une chaine de caractere en un integer
//@param rien
//@return rien
int my_atoi(const char *str);

//Converti un integer en chaine de caractere
//@param n entier a convertir
//@param dst addresse de la chaine ou sera mis le resultat 
//@return dst addresse de la chaine ou sera mis le resultat passé en parametre
char *my_itoa(const int n, char *dst);