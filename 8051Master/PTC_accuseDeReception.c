//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_accuseDeReception.c
// Destination: 8150F020 Carte Master
// Description: Fonctions qui gere les accuses de receptions
//------------------------------------------------------
#include "PTC_accuseDeReception.h"
#include "PTC_UART.h"
#include "UART0_RingBuffer_lib.h"

void AR_cmd_incorrecte(){//envoi de <CR><LF>#
    serOutchar(0x0D);
    serOutchar(0x0A);
    serOutchar(0x23);
}

void AR_cmd_correcte(){ //envoi de <CR><LF>> 
    serOutchar(0x0D);
    serOutchar(0x0A);
    serOutchar(0x3E);
}