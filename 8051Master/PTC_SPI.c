
#include "PTC_SPI.h"
#include "SPI_RingBuffer_Master.h"

int Rx_chaine_SPI(char *Pchaine_courante){
	char c;
	if ((c=serInchar_SPI()) != 0){//si on vient de recevoir un caractere
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
