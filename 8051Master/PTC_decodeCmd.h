//------------------------------------------------------
// Date: 01/04/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.h
// Destination: 8150F020 Carte Master
// Description: Fonctions qui permette de cecomposer les commandes du PC de commande
//------------------------------------------------------



//Fonction qui analise et traite la commande D (Debut de l'epreuve)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_D (const char *Pchaine_courante);

//Fonction qui analise et traite la commande E (Fin de l'epreuve)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_E (const char *Pchaine_courante);

//Fonction qui analise et traite la commande A (Avancer)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_A (const char *Pchaine_courante);

//Fonction qui analise et traite la commande B (Reculer)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_B (const char *Pchaine_courante);

//Fonction qui analise et traite la commande RG (Rotation de 90 vers la gauche)
void Cmd_epreuve_RG(void);

//Fonction qui analise et traite la commande RD (Rotation de 90 vers la droite)
void Cmd_epreuve_RD(void);

//Fonction qui analise et traite la commande RC (Rotation de 180)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_RC(const char *Pchaine_courante);

//Fonction qui analise et traite la commande RA (Rotation d'un angle donne)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_RA(const char *Pchaine_courante);

//Fonction qui analise et traite la commande S (Stop)
void Cmd_epreuve_S (void);

//Fonction qui analise et traite la commande G (Rejoint les coordonnees indique)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_G (const char *Pchaine_courante);

//Fonction qui analise et traite la commande TV (Modifie la vitesse de deplacement par default)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_TV(const char *Pchaine_courante);

//Fonction qui analise et traite la commande CS (Positionnement des cervos-moteurs)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_CS(const char *Pchaine_courante);

//Fonction qui analise et traite la commande L (commande du pointeur lumineux)
//@param Pchaine_courante la chaine complete du message recu
void Cmd_epreuve_L(const char *Pchaine_courante);