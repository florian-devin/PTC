//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_accuseDeReception_SPI.c
// Destination: 8150F020 Carte Slave
// Description: Fonctions qui gere les accuses de receptions
//------------------------------------------------------
#include "PTC_accuseDeReception_SPI.h"
#include "SPI_RingBuffer_Slave.h"

void AR_cmd_incorrecte_SPI(){//envoi de #
    serOutchar_SPI(0x23);
}

void AR_cmd_correcte_SPI(){ //envoi de > 
    serOutchar_SPI(0x3E);
}