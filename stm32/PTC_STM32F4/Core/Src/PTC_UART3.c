#include "PTC_UART3.h"

extern char *rx_Buf;
extern int   prx_Buf;

int Rx_chaine(char *Pchaine_courante){
	char c;
	if ((c=serInchar_uart3_stm32()) != 0){//TODO a modifier 
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


char serInchar_uart3_stm32(void) {
    char data = rx_Buf[prx_Buf];
    if (data != 0x00) {
  	    rx_Buf[prx_Buf++] = 0x00;      
        if (prx_Buf > 255)
            prx_Buf = 0;
  	    return data;
    } 
  else return 0;
}
