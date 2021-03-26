//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: main.c
// Destination: 8150F020 Carte Master
// Description: Fonctions main
//------------------------------------------------------

#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "config_globale.h"
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_geter_cmd.h"
#include "PTC_strOperateurs.h"
#include "PTC_timer.h"
#include "PTC_UART.h"
#include "PTC_deplacement.h"
#include "PTC_math.h"
#include "PTC_detection.h"

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
	int i;
	//Send_str("S");
		Send_str_uart1("reset\r");
		for(i=0;i<20000;i++){}
		//Send_str_uart1("clrenc 1 2\r");
		for(i=0;i<20000;i++){}
		Send_str_uart1("cfg enc 1\r");
		for(i=0;i<20000;i++){}
			
	  
		for(i=0;i<10000;i++){}
		Send_str_uart1("digo 1:582:6\r");
		for(i=0;i<10000;i++){}

//	Send_str_uart1("a");
//	Send_str_uart1("stop\r");
//	for(i=0;i<10000;i++){}
//	Send_str_uart1("cfg enc\r");
//	for(i=0;i<10000;i++){}
//	Send_str_uart1("clrenc\r");
//	//Send_str("reset\r");
//	for(i=0;i<10000;i++){}
//	get_encoder("1");
//		for(i=0;i<10000;i++){}
//		//Send_str("mogo 1:99 2:99\r");
//	Avancer("6");
//			for(i=0;i<10000;i++){}
//			get_encoder("1");
	//for(i=0;i<2000;i++){Delay_1ms();}
	//Stop();
}
	
void loop() {
//	if (Rx_chaine(chaine_courante) == 1){
//		decodage_commande(chaine_courante);
//	}

}

//void decodage_commande(char *Pchaine_courante){ //fonction qui decode les commades et les applique
//	char commande[7] = {0};
//	Send_str("La chaine est :");
//	Send_str(Pchaine_courante);
//	Send_str("\r\n");
//	get_commande(Pchaine_courante,commande);
//	
//	if (my_strcmp(commande,"A")){ //Avancer
//		char str_param[12] = {0};
//		int param = 0;
//		Send_str("Avancer");
//		get_param(Pchaine_courante,1, str_param);
//		param = my_atoi(str_param);
//		if( param <= 100 && param >= 5){
//			char chaine[64] = "mogo 1:";
//			AR_cmd_correcte();
//			my_strcat(chaine,str_param);
//			my_strcat(chaine," 2:");
//			my_strcat(chaine,str_param);
//			my_strcat(chaine,"\r");
//			Send_str_uart1("\r"); //remet a 0 le serializer
//			Send_str_uart1(chaine); //evoie du message
//		}
//		else {
//			AR_cmd_incorrecte();
//		}
//	}

//	else if (my_strcmp(commande,"S")){ //Stop
//		AR_cmd_correcte();
//		Send_str_uart1("\r"); //remet a 0 le serializer
//		Send_str_uart1("stop\r");
//	}
//	else {
//		AR_cmd_incorrecte();
//	}
//	
//	RAZ_str(Pchaine_courante);
//}

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


















