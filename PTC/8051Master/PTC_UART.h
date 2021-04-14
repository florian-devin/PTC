//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_UART.h
// Destination: 8150F020 Carte Master
// Description: Fonctions qui gere les UART
//------------------------------------------------------

//Place Flag_TX0 a 1
//@param rien
//@return rien
void Flag_TX0_fct(void);

//Envoi une chaine via l'uart0
//@param str chaine a envoyer
//@return rien
void Send_str(char *str);

//Envoi un caractere via l'uart0
//@param caractere caractere a envoyer
//@return rien
void Send_char(const char caractere);

//Indique si on vien de recevoir un message complet via l'uart0 et complete la chaine courante si un nouveau caractere est arrive
//@param Pchaine_courante chaine qu'on est en train de recevoir
//@return 1 si la chaine est fini | 0 sinon
int Rx_chaine(char *Pchaine_courante);

//Routine quand on recoit un caractere sur l'uart0 appeler dans l'interruption
//@param Pcaractere_recu le caractere recus
//@return rien
void caractere_recu_fct_uart0(char Pcaractere_recu);

//Routine quand on recoit un caractere sur l'uart1 appeler dans l'interruption
//@param Pcaractere_recu le caractere recus
//@return rien
void caractere_recu_fct_uart1(char Pcaractere_recu);

//Envoi un caractere via l'uart1
//@param caractere caractere a envoyer
//@return rien
void Send_char_uart1(const char caractere);

//Envoi une chaine via l'uart1
//@param str chaine a envoyer
//@return rien
void Send_str_uart1(char *str);

//Place Flag_TX1 a 1
//@param rien
//@return rien
void Flag_TX1_fct(void);

//Place Flag_RX1 a 1
//@param rien
//@return rien
void Flag_RX1_fct(void);


//attend l'accuse de reception du robot et dit si il est valide
//@return 1 si ACK 0 si NACK -1 si autre
char Wait_Accuse_RX_Robot(void);