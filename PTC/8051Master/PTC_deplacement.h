//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_deplacement.h
// Destination: 8150F020 Carte Master
// Description: Fonctions de deplacement elementaires 
//------------------------------------------------------

//Demmande au serializer d'avancer a la vitesse str_vitesse
//@param vitesse sous forme d'un char *
void Avancer(char *str_vitesse);

//Demmande au serializer d'avancer a la vitesse str_vitesse
//@param vitesse sous forme d'un char *
void Reculer(char *str_vitesse);

//Demmande au serializer darreter les moteurs
void Stop(void);

//Demande au serializer le compteur de l'encoder Id
//@param Id Id de l'encoder ("1" ou "2")
//@return le compteur 
long get_encoder(char *Id);

//Demande au serializer le reinitialiser le compteur de l'encoder Id
//@param Id Id de l'encoder ("1" ou "2")
void RAZ_encoder(char *Id);

//Effectue un angle a droite (TODO)
//@param angle en degre
void turn_right(int angle);

//Effectue un angle a gauche (TODO)
//@param angle en degre
void turn_left(int angle);

//Permet au robot de rejoindre les coordonnees indiques les robot peut eviter les obstacles
//@param coord_x coordonnee X en cm 
//@param coord_y coordonnee Y en cm
//@param angle angle de possitionnement du robot par rapport au debut
void go_coordinates_without_obstacles(int coord_x, int coord_y, int angle);


