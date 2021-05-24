//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_geter_cmd.c
// Destination: 8150F020 Carte Master
// Description: Fonctions qui recupere les informations d'une chaine de caractere
//------------------------------------------------------
#include "PTC_geter_cmd.h"
#include <string.h>

//Decoupage de la trame en plusieur commande
//      - nom de la commande (D,E,Q,TV,A,B,S,RD,RG,RC,RA,G,ASS,MI,ME,IPO,POS,MOU,MOB,MOS,SD,L,LS,CS,PPH,SPH,AUX) get_commande()
//      - parametres (B,12,D:21, etc) get_param()
//get_commande(const char *str, char *commande) : place dans commande le nom de la commande recue
//get_param(const char *str,const unsigned char position, char *param) : place dans param le potition eme parametre qu'il soit complexe ou simple 
//get_complex_param(const char *param, char *name_param, char *value_param) : place le nom et la valeur du parametre dans name_param et valu_param
// A 56 
void get_commande(const char *str, char *commande){
    unsigned char i = 0;
    while (*str != '\0' && *str != 0x20){
        commande[i] = *str;
        i++;
        str++;
    }
}

void get_param(const char *str,const unsigned char position, char *param){
    unsigned char i = 0;
    unsigned char param_courant = 1;
    while (*str != '\0' && *str != 0x20){ //rien ou un espace
        str++;
    }
    str++;
    for (param_courant = 1; param_courant <= position; param_courant++){
        char caractere;
        char len_param = (char)strlen(param);
		for (caractere = 0; caractere <= len_param; caractere++){//RAZ de param
            param[caractere] = '\0';
        }
        i=0;
        while(*str != '\0' && *str != 0x20){ //rien ou espace
            param[i] = *str;
            i++;
            str++;
        }
        str++;
    }
}

void get_complex_param(const char *param, char *name_param, char *value_param){
    unsigned char i = 0;
    while (*param != '\0' && *param != 0x3A){
        name_param[i] = *param;
        i++;
        param++;
    }
    param++;
    i=0;
    while (*param != '\0' && *param != 0x20){
        value_param[i] = *param;
        i++;
        param++;
    }
}
