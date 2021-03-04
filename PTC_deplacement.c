//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_deplacement.c
// Destination: 8150F020 Carte Master
// Description: Fonctions de deplacement elementaires 
//------------------------------------------------------

#include "PTC_deplacement.h"
#include "PTC_strOperateurs.h"  // pour my_strcat
#include "PTC_UART.h" // pour Send_str_uart1




void Avancer(char *str_vitesse){
    char data[64] = "mogo 1:";
	my_strcat(data,str_vitesse);
	my_strcat(data," 2:");
	my_strcat(data,str_vitesse);
	my_strcat(data,"\r");
	Send_str_uart1("\r"); //remet a 0 le serializer
	Send_str_uart1(data); //evoie du message
}

void Stop(void){
    Send_str_uart1("\r"); //remet a 0 le serializer
	Send_str_uart1("stop\r");
}