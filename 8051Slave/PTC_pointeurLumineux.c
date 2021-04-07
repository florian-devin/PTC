//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_pointeurLumineux.c
// Destination: 8150F020 Carte Slave
// Description: Fonctions d'allumage du pointeur lumineux
//------------------------------------------------------


#include <C8051F020.h>
#include "c8051F020_SFR16.h"
#include "PTC_pointeurLumineux.h"
#include "PTC_timer.h"
#include "PTC_PWM.h"


extern unsigned char    Lumiere_Lum_Nbre;
extern unsigned int     Lumiere_Lum_OFF;
extern unsigned int     Lumiere_Lum_ON;
extern unsigned char    Lumiere_Intensite;
extern char              Lumiere_loop_Enable;    //1 si il faut faire Lumiere_loop 0 sinon
char                     Etat_LED            = 0;//Etat de la LED pour Lumiere_loop
unsigned int            Temp_init;              //Temps de reference pour Lumiere_loop


void Lumiere (const unsigned char Intensite, const unsigned int Lum_ON, const unsigned int Lum_OFF, const unsigned char Lum_Nbre) {
    Lumiere_loop_Enable = 1;
    Lumiere_Lum_Nbre    = Lum_Nbre;
    Lumiere_Lum_OFF     = Lum_OFF;
    Lumiere_Lum_ON      = Lum_ON;
    Lumiere_Intensite   = Intensite;
    Temp_init           = get_time_ms();
}

void Lumiere_Stop (void) {
    analogWrite_CEX0(0); //On eteind la LED
    Lumiere_loop_Enable = 0; //on arrete le processus de clignautement
}
/*
void Lumiere_loop (const unsigned char Intensite, const unsigned int Lum_ON, const unsigned int Lum_OFF, unsigned char *Lum_Nbre) {
    if (Lum_Nbre != 0) {
        if (Etat_LED) { //si la led est allume 
            if ((get_time_ms() - Temp_init) > Lum_ON) {
                analogWrite_CEX0(Intensite); //On alume la LED
                Etat_LED = 1; //on signal l'etat de la LED
                Temp_init = get_time_ms(); //Nouvelle reference de temps
            }
        }
        else {
            if ((get_time_ms() - Temp_init) > Lum_OFF) {
                analogWrite_CEX0(0); //On eteind la LED
                Etat_LED = 1; //on signal l'etat de la LED
                Temp_init = get_time_ms(); //Nouvelle reference de temps
                Lum_Nbre--; //On decremente le nombre de cycle
                if (Lum_Nbre == 0)
                    Lumiere_loop_Enable = 0;
            }
        }
    }
}*/

void Lumiere_loop (void) {
    if (Lumiere_Lum_Nbre != 0) {
        if (!Etat_LED) { //si la led est allume 
            if ((get_time_ms() - Temp_init) > Lumiere_Lum_ON) {
                analogWrite_CEX0(Lumiere_Intensite); //On alume la LED
                Etat_LED = 1; //on signal l'etat de la LED
                Temp_init = get_time_ms(); //Nouvelle reference de temps
            }
        }
        else {
            if ((get_time_ms() - Temp_init) > Lumiere_Lum_OFF) {
                analogWrite_CEX0(0); //On eteind la LED
                Etat_LED = 0; //on signal l'etat de la LED
                Temp_init = get_time_ms(); //Nouvelle reference de temps
                Lumiere_Lum_Nbre--; //On decremente le nombre de cycle
                if (Lumiere_Lum_Nbre == 0)
                    Lumiere_loop_Enable = 0;
            }
        }
    }
}