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

#include "UART0_RingBuffer_lib.h"
#include "PTC_strOperateurs.h"
#include "PTC_convertion.h"

extern unsigned int     Lumiere_Lum_Nbre;
extern unsigned int     Lumiere_Lum_OFF;
extern unsigned int     Lumiere_Lum_ON;
extern unsigned int     Lumiere_Intensite;
extern char             Lumiere_loop_Enable;    //1 si il faut faire Lumiere_loop 0 sinon
char                    Etat_LED            = 0;//Etat de la LED pour Lumiere_loop
unsigned long           Temp_init;              //Temps de reference pour Lumiere_loop


void Lumiere (const unsigned char Intensite, const unsigned int Lum_ON, const unsigned int Lum_OFF, const unsigned char Lum_Nbre) {
    char str_debug[128] = "La lumiere s'allume : Lum_Nbre = ";
    char str_inter[10]  = {0};
    Lumiere_loop_Enable = 1;
    Lumiere_Lum_Nbre    = Lum_Nbre;
    Lumiere_Lum_OFF     = Lum_OFF;
    Lumiere_Lum_ON      = Lum_ON;
    Lumiere_Intensite   = Intensite;
    Temp_init           = get_time_ms();

    my_strcat(str_debug,my_itoa(Lum_Nbre,str_inter));
    my_strcat(str_debug," Lum_OFF = ");
    my_strcat(str_debug,my_itoa(Lum_OFF,str_inter));
    my_strcat(str_debug," Lum_ON = ");
    my_strcat(str_debug,my_itoa(Lum_ON,str_inter));
    my_strcat(str_debug," Intensite = ");
    my_strcat(str_debug,my_itoa(Intensite,str_inter));
    my_strcat(str_debug,"\r\n");
    serOutstring(str_debug);

}

void Lumiere_Stop (void) {
    analogWrite_CEX0(0); //On eteind la LED
		Etat_LED = 0;
    Lumiere_loop_Enable = 0; //on arrete le processus de clignotement
}


void Lumiere_loop (void) {
    if (Lumiere_Lum_Nbre != 0) {
        if (!Etat_LED) { //si la led est eteinte
            if ((get_time_ms() - Temp_init) > Lumiere_Lum_OFF) {
				unsigned char value = (Lumiere_Intensite*255/100);
                analogWrite_CEX0(value); //On allume la LED
                Etat_LED = 1; //on signal l'etat de la LED
                Temp_init = get_time_ms(); //Nouvelle reference de temps
            }
        }
        else {
			long tmp = get_time_ms() - Temp_init; //debug
            if ((get_time_ms() - Temp_init) > Lumiere_Lum_ON) {
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