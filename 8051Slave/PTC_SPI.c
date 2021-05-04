//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_SPI.c
// Destination: 8150F020 Carte Slave
// Description: Fonctions qui gere le SPI
//------------------------------------------------------


#include "PTC_SPI.h"
#include "SPI_RingBuffer_Slave.h"
#include "c8051F020_SFR16.h"
#include "c8051F020.h"

//*****************************************************************************	 
//*****************************************************************************	 

// Rx_chaine -- retourne 1 si on recoit un message complet.
int Rx_chaine_SPI(char *Pchaine_courante){
	char c = serInchar_SPI();
	if (c != 0 && c!=0x01){//si on vient de recevoir un caractere
		int i = 0; //cpt position de chaine
		if(c == '\r'){
			return 1;
		}
		while(Pchaine_courante[i] != '\0'){
		    i++;
		}
		Pchaine_courante[i] = c; //on complete la chaine de caractere
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


