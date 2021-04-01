//------------------------------------------------------
// Date: 01/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: SPI_RingBuffer_Slave.h
// Destination: 8150F020 Carte Master
// Description: Fonctions pour l'SPI avec buffer circ
//------------------------------------------------------
sbit SS = P1^0; //Slave-Select pour le SPI
//Initialisation des structuresde gestion des buffers transmission et reception
void init_Serial_Buffer_SPI(void);


//envoi d'un caractere dans le buffer de transmission de la liaison SPI
//@param c caractere
//@return 1 si ok 0 si error
unsigned char serOutchar_SPI(char c);

//Lecture d'un caractere dans le buffer de reception de la liaison SPI
//Fonction adaptee pour la reception de codes ASCII - La reception de la valeur binaire 0 
//n'est pas possible (conflit avec le code 0 retourne si il n'y a pas de caractere recu)
//@param buf chaine a envoyer
//@return le caractere ou 0 si il n'y a pas de caractere recu
char serInchar_SPI(void); 

//lecture d'un caractere dans le buffer de reception de la liaison SPI
//  Fonction adaptee pour la reception de codes Binaires - Cette fonction retourne un entier. 
//  L'octet de poids faible correspond au caractere reeu, l'octet de poids fort, s'il est nul indique 
//  qu'aucun caractere n'a ete recu.
//@return un code d'erreur
unsigned int serInchar_Bin_SPI(void);

//Envoi d'une chaine de caractere dans le buffer de transmission
//@param buf chaine a envoyer
//@return code d'erreur
unsigned char serOutstring_SPI(char *buf);
