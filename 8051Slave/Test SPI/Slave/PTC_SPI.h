//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_SPI.h
// Destination: 8150F020 Carte Slave
// Description: Fonctions qui gere le SPI
//------------------------------------------------------


// Rx_chaine -- retourne 1 si on recoit un message complet.
//@param Pchaine_courante l'adresse de la chaine courante
//@return 1 si chaine complete sinon 0
int Rx_chaine_SPI(char *Pchaine_courante);