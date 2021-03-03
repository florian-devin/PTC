//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// AUTH: Florian DEVIN
// DATE: 24/02/2021
//
// Target: C8051F02x
// Tool chain: KEIL Microvision 4
//
//-----------------------------------------------------------------------------

#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "config_globale.h"
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_geter_cmd.h"
#include "PTC_strOperateurs.h"
#include "PTC_timer.h"
#include "PTC_UART.h"

//-----------------------------------------------------------------------------
//variables globales
//En lien avec l'UART0
//char interrupt_out = 0;        //mon flag pour le caractere recu
//char caractere_recu = '\0';    //caractere qui vient d'etre recu
char chaine_courante[64] = {0};//chaine total qui vas contenir le mot recu (20 caractere max)



//En lien avec le temp
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//headers
//void main();
void setup();
void startup();
void loop();




//-----------------------------------------------------------------------------


void main(){
    setup();
    startup();
	
    while(1) {
        loop();
    }
}

void setup(){
    Init_Device(); //fonction de config_globale.c
}


void startup(){
	Send_str("Salut0\r\n");
	Send_str_uart1("Salut1\r\n");
}
	
void loop() {
	if (Rx_chaine(chaine_courante) == 1){
		char commande[7] = {0};
		Send_str("La chaine est :");
		Send_str(chaine_courante);
		Send_str("\r\n");
		get_commande(chaine_courante,commande);
		
		if (my_strcmp(commande,"A")){
			char str_param[12] = {0};
			int param = 0;
			Send_str("Avancer");
			get_param(chaine_courante,1, str_param);
			param = my_atoi(str_param);
			if( param <= 100 && param >= 5){
				Send_str("a la vitesse : ");
				Send_str(str_param);
				Send_str("\r\n");
				AR_cmd_correcte();
				Send_str_uart1("mogo 1:45 2:45\r\n");
			}
			else {
				AR_cmd_incorrecte();
			}
		}
		else if (my_strcmp(commande,"S")){
			Send_str("Stop\r\n");
			AR_cmd_correcte();
			Send_str_uart1("stop\r\n");
		}
		else {
			AR_cmd_incorrecte();
		}
		
		RAZ_str(chaine_courante);
	}
	//str_cutting("A 8 2:45 B:32");
}



//void Interrupt_Time(void) interrupt 1 {//interruption declancher par l'overflow du Timer 0 (toutes les us)
//    TF0 = 0; //interrupt flag
//    Time_increment();
//}

void Interrupt_UART0(void) interrupt 4 { //interruption declanch�e par l'UART0
    if (RI0 == 1){//si on vient de recevoir un caractere
			caractere_recu_fct_uart0(SBUF0);
      //caractere_recu = SBUF0;
    }
		else {
			Flag_TX0_fct();
		}
			
	  RI0 = 0; //Rx flag
		TI0 = 0;
		
}


void Interrupt_UART1(void) interrupt 20 { //interruption declanch�e par l'UART0
    if ((SCON1 & 0x01) == 0x01){//si on vient de recevoir un caractere
			caractere_recu_fct_uart1(SBUF1);
    }
		else {
			Flag_TX1_fct();
		}
		SCON1 &= ~(1<<0);//Rx flag
		SCON1 &= ~(1<<1);//Tx flag
}


















