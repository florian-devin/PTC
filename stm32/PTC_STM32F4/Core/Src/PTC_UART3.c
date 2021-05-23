#include "PTC_UART3.h"
#include "UART0_RingBuffer_lib.h"


int Rx_chaine(char *Pchaine_courante){
	char c;
	if ((c=serInchar()) != 0){
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

//remet str a rien
void RAZ_str(char *str){ 
	while(*str != '\0') {
		*str = '\0';
		str++;
    }
}

