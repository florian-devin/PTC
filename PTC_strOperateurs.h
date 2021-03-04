//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_strOperateurs.h
// Destination: 8150F020 Carte Master
// Description: Fonctions d'operation sur les chaine de caractere
//------------------------------------------------------

//Compare si deux chaines sont egaux
//@param s1 chaine de caractere a comparer
//@param s2 chaine de caractere a comparer
//@return 1 si s1 = s2 |  0 sinon
int my_strcmp(const char *s1, const char *s2);

//remet a zero (\0) une chaine de caractere
//@param s1 chaine de caractere a reseter
//@return rien
void RAZ_str(char *str);

//Taille d'une chaine de caractere
//@param str chaine de caractere a meusurer
//@return La taille de la chaine
size_t my_strlen(char const *str);

//Concatene deux chaines
//@param s1 1er chaine et chaine qui recevra s1+s2
//@param s2 chaine a concatener
//@return la chaine s1 resulat de la concatenation
char *my_strcat(char *s1, const char *s2);