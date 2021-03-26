//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_UART.c
// Destination: 8150F020 Carte Master
// Description: Fonctions qui gere les UART
//------------------------------------------------------


#include "PTC_UART.h"
#include "c8051F020_SFR16.h"
#include "c8051F020.h"

#define SIZE_OF_BUFFER8_UART0 64
#define SIZE_OF_BUFFER8_UART1 64
//*****************************************************************************	 
//*****************************************************************************	 
// Send_char -- Envoi d'un caractere sur l'UART0
// Send_str  -- Envoi d'une chaine de caractere

//UART0
char Flag_TX0 = 0;
char Flag_RX1 = 0;

char caractere_recu = '\0';    //caractere qui vient d'etre recu
char interrupt_out = 0;
//circular buffer
char bufferLength_uart0 = 0;
char readIndex_uart0 = 0; 
char writeIndex_uart0= 0;

//UART1
char Flag_TX1 = 0;
char caractere_recu_uart1 = '\0';    //caractere qui vient d'etre recu
char interrupt_out_uart1 = 0;
//circular buffer
char bufferLength_uart1 = 0;
char readIndex_uart1 = 0; 
char writeIndex_uart1= 0;

void Flag_TX0_fct(void){
	Flag_TX0 = 1;
}

void Send_char(const char caractere){
	REN0 = 0; //desactivation de Rx
	SBUF0 = caractere;
	while(Flag_TX0 == 0);
	Flag_TX0 = 0; //Tx flag
	REN0 = 1; //activation de Rx
}

void Send_str(char *str){
	unsigned char i = 0;
	while(*str!= '\0') { //tant qu'on est pas a la fin de la chaine
		Send_char(*str);
		str++;
		i++;
	}
}

//*****************************************************************************	 
//*****************************************************************************	 
// Rx_chaine -- retourne 1 si on recoit un message complet.

int Rx_chaine(char *Pchaine_courante){
    if(interrupt_out == 1){ //si on vient de recevoir un caractere
			int i = 0; //cpt position de chaine
			interrupt_out = 0; //raz flag
			if(caractere_recu == '\r'){
					//Pcaractere_recu = '\0';
					return 1;
			}
			while(Pchaine_courante[i] != '\0'){
		    i++;
			}
			Pchaine_courante[i] = caractere_recu; //on complete la chaine de caractere
			//Pcaractere_recu = '\0';
		}
	return 0;
}

void caractere_recu_fct_uart0(char Pcaractere_recu){
	caractere_recu = Pcaractere_recu;
	interrupt_out = 1;
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void caractere_recu_fct_uart1(char Pcaractere_recu){
	caractere_recu_uart1 = Pcaractere_recu;
	interrupt_out_uart1 = 1;
}

void Send_char_uart1(const char caractere){
	SCON1 &= ~(1<<4); //desactivation de Rx
	SBUF1 = caractere;
	while(Flag_TX1 == 0);
	Flag_TX1 = 0; //Tx flag
	SCON1 |= (1<<4); //activation de Rx
}
void Send_str_uart1(char *str){
	unsigned char i = 0;
	while(*str!= '\0') { //tant qu'on est pas a la fin de la chaine
		Send_char_uart1(*str);
		str++;
		i++;
	}
}
void Flag_TX1_fct(void){
	Flag_TX1 = 1;
}

void Flag_RX1_fct(void){
	Flag_RX1 = 1;
}

void Accuse_RX_Robot(void){

}