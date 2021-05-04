//Fichier de test pour dev la liaison SPI 



#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "slave_config_globale.h"
#include "SPI_RingBuffer_Slave.h"
#include "UART0_RingBuffer_lib.h"
#include "PTC_SPI.h"
#include "PTC_strOperateurs.h"
#include "PTC_geter_cmd.h"

#ifndef CFG_Globale
   #define CFG_Globale
   #define SYSCLK           22118400 //approximate SYSCLK frequency in Hz
#endif


void decodage_commande(char *Pchaine_courante_SPI);
//--------------------------Variables Globales
//En lien avec la SPI
char chaine_courante_SPI[64] = {0};//chaine total qui vas contenir le mot recu
//-------------------Declaration des ports E/S

//--------------------------------------------
//------------------------------Configurations



//-------------------------------------------


//-----------------------Programme principale


void setup() {
    Init_Slave();
    init_Serial_Buffer();//UART0
    init_Serial_Buffer_SPI();
    EA    = 1; //interruption general enable
}


void loop() {
    if (Rx_chaine_SPI(chaine_courante_SPI)) {
        serOutstring("Rx = 1\r\n");
        decodage_commande(chaine_courante_SPI);
    }
}

void decodage_commande(char *Pchaine_courante_SPI){ //fonction qui decode les commades et les applique
	char commande[7] = {0};
	get_commande(Pchaine_courante_SPI,commande);
    if (my_strcmp(commande,"SPI")) { //Pour le test de la liaison SPI
		serOutstring("Commande SPI bien recu !\r\n");
        serOutstring_SPI("Cmd SPI ok");
	}
	RAZ_str(Pchaine_courante_SPI);
}

void startup() {
    serOutstring("UART Slave Ready \r\n");
	while(serInchar_SPI() != 0x01);
	serOutchar_SPI(0x02);
	serOutstring("SPI Slave Ready \r\n");
} 


void main(){
    setup();
    startup();
    while(1) {
        loop();
    }
}

