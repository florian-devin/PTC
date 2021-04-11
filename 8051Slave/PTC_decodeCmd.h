//------------------------------------------------------
// Date: 01/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.h
// Destination: 8150F020 Carte Master
// Description: Fonctions qui permette de cecomposer les commandes du PC de commande
//------------------------------------------------------

//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.c
// Destination: 8150F020 Carte Master
// Description: Fonctions qui permette de cecomposer les commandes du PC de commande
//------------------------------------------------------
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_deplacement.h"
#include "PTC_geter_cmd.h"
#include "PTC_servoMoteurHorizontal.h"
#include "PTC_strOperateurs.h"
#include "UART0_RingBuffer_lib.h"
#include "SPI_RingBuffer_Master.h"
//------------------------------------------------------
//------------------------------------Variables Globales
//extern int              epreuve_enable; //unused

extern unsigned char    temp_servo_V;
extern char             flag_print_arrive_servo_V;

//------------------------------------------------------
//------------------------------------------------------


//Fonction qui analise et traite la commande CS (Positionnement des cervos-moteurs)
//@param Pchaine_courante_SPI la chaine complete du message recu
void Cmd_epreuve_CS_Slave(const char *Pchaine_courante_SPI);

//Fonction qui analise et traite la commande L (commande du pointeur lumineux)
//@param Pchaine_courante_SPI la chaine complete du message recu
void Cmd_epreuve_L_Slave(const char *Pchaine_courante_SPI);

//Fonction qui analise et traite la commande LS (commande extinction du pointeur lumineux)
//@param Pchaine_courante_SPI la chaine complete du message recu
void Cmd_epreuve_LS_Slave(const char *Pchaine_courante_SPI);

//Fonction qui analise et traite la commande ASS (Aquisition de signaux sonores)
//@param Pchaine_courante_SPI la chaine complete du message recu
void Cmd_epreuve_ASS_Slave(const char *Pchaine_courante_SPI);

//Fonction qui analise et traite la commande ASS (Generation de signaux sonore)
//@param Pchaine_courante_SPI la chaine complete du message recu
void Cmd_epreuve_SD_Slave(const char *Pchaine_courante_SPI);



