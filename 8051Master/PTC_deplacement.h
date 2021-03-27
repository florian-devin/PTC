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

int get_encoder(char Id);

//Effectue un angle a droite (TODO)
//@param angle en degre
//@return rien
void turn_right(int angle);

//Effectue un angle a gauche (TODO)
//@param angle en degre
//@return rien
void turn_left(int angle);

//Permet au robot de rejoindre les coordonnees indiques les robot peut eviter les obstacles
//@param coord_x coordonnee X en cm 
//@param coord_y coordonnee Y en cm
void go_coordinates_without_obstacles(int coord_x, int coord_y);


/*
//Permet au robot de rejoindre les coordonnees indiques les robot peut eviter les obstacles
//@param coord_x coordonnee X en cm 
//@param coord_y coordonnee Y en cm
//@return 1 si on est arrive 0 sinon
int go_coordinates(int coord_x, int coord_y);

//Contourne l'obstacle (fonction bloquante a ameliorer) //TODO : rendre cette fonction plus blocante
//@param void
//@return void
void bypass_obstacle();

*/
