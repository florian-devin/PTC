//------------------------------------------------------
// Date: 02/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_PWM.c
// Destination: 8150F020 Carte Slave
// Description: Fonctions de generation de signaux pwm
//------------------------------------------------------
#include "c8051F020.h"
#include "c8051F020_SFR16.h"
#include "PTC_PWM.h"



void analogWrite_CEX0(const unsigned char value) {
    if (!value)
        PCA0CPM0 &= ~(1<<6); //ECOM0 = 0 pour desactiver le PWM
    else {
        PCA0CPM0 |= (1<<6); //ECOM0 = 1 pour activer le PWM
        PCA0CPH0 = 0xFF - value;
    }
}


void analogWrite_CEX1(const unsigned int value) {
    if (!value)
        PCA0CPM1 &= ~(1<<6); //ECOM0 = 0 pour desactiver le PWM
    else {
        PCA0CPM1 |= (1<<6); //ECOM0 = 1 pour activer le PWM
        PCA0CPL1 = 0xFFFF - value;
        PCA0CPH1 = (0xFFFF - value) >> 8;
    }
}