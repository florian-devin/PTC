//------------------------------------------------------
// Date: 26/03/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: UART0_RingBuffer_lib.h
// Destination: 8150F020 Carte Master
// Description: Fonctions pour l'uart0 avec buffer circ
//------------------------------------------------------

//Initialisation des structuresde gestion des buffers transmission et reception
void init_Serial_Buffer(void);


//Lecture d'un caractere dans le buffer de reception de la liaison serie. 
//Fonction adaptee pour la reception de codes ASCII - La reception de la valeur binaire 0 
//n'est pas possible (conflit avec le code 0 retourne si il n'y a pas de caractere recu)
//@param buf chaine a envoyer
//@return le caractere ou 0 si il n'y a pas de caractere recu
char serInchar(void); 

//lecture d'un caractere dans le buffer de reception de la liaison serie
//  Fonction adaptee pour la reception de codes Binaires - Cette fonction retourne un entier. 
//  L'octet de poids faible correspond au caractere reeu, l'octet de poids fort, s'il est nul indique 
//  qu'aucun caractere n'a ete recu.
//@return un code d'erreur
unsigned int serInchar_Bin(void);

