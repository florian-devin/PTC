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

void AR_cmd_incorrecte(){//envoi de <CR><LF>#
    Send_char(0x0D);
    Send_char(0x0A);
    Send_char(0x23);
}

void AR_cmd_correcte(){ //envoi de <CR><LF>> 
    Send_char(0x0D);
    Send_char(0x0A);
    Send_char(0x3E);
}