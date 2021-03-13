//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_geter_cmd.h
// Destination: 8150F020 Carte Master
// Description: Fonctions qui recupere les informations d'une chaine de caractere
//------------------------------------------------------

//Decoupage de la trame en plusieur commande
//      - nom de la commande (D,E,Q,TV,A,B,S,RD,RG,RC,RA,G,ASS,MI,ME,IPO,POS,MOU,MOB,MOS,SD,L,LS,CS,PPH,SPH,AUX) get_commande()
//      - parametres (B,12,D:21, etc) get_param()
//get_commande(const char *str, char *commande) : place dans commande le nom de la commande recue
//get_param(const char *str,const unsigned char position, char *param) : place dans param le potition eme parametre qu'il soit complexe ou simple 
//get_complex_param(const char *param, char *name_param, char *value_param) : place le nom et la valeur du parametre dans name_param et valu_param

//Recupere la commade
//@param str  chaine de caractere complete
//@param commande chaine de caractere dans laquelle est retourné la commande
//@return rien
void get_commande(const char *str, char *commande);

//Recupere un parametre
//@param str  chaine de caractere complete
//@param position position du parametre dans la chaine de caractere complete
//@param param chaine de caractere dans laquelle est retourné le parametre
//@return rien
void get_param(const char *str,const unsigned char position, char *param);

//Recupere les informations d'un parametre complexe
//@param param  chaine de caractere du parametre complexe
//@param name_param chaine de caractere dans laquelle est retourné le nom du parametre
//@param value_param chaine de caractere dans laquelle est retourné la valeurs du parametre
//@return rien
void get_complex_param(const char *param, char *name_param, char *value_param);