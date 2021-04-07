//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: main.c
// Destination: 8150F020 Carte Slave
// Description: Programme principal de la carte slave
//------------------------------------------------------

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "config_globale.h"
#include "PTC_timer.h"
#include "PTC_pointeurLumineux.h"
#include "PTC_PWM.h"



char            Lumiere_loop_Enable = 0; //1 si il faut faire Lumiere_loop 0 sinon
unsigned char   Lumiere_Intensite, Lumiere_Lum_Nbre;
unsigned int    Lumiere_Lum_ON, Lumiere_Lum_OFF;



void setup(void) {
    Init_Device();
		Lumiere(90,1000,1000,10);

}

void loop(void) {
   ////----------------------test de PWM
   //unsigned char i;
   //for(i = 0; i <= 255; i++) {
   //    analogWrite_CEX0(i);
   //    Delay(50);
   //}
   ////---------------------------------


    if (Lumiere_loop_Enable) //Routine alumage lumiere
        Lumiere_loop();

    
}

void main(void) {
setup();
while(1){
    loop();
}
}





void Interrupt_Time(void) interrupt 16 {//interruption declancher par l'overflow du Timer 3 (toutes les ms)
	T4CON &= ~(1<<7); //interrupt flag
    Time_increment();
}