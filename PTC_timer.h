//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_timer.h
// Destination: 8150F020 Carte Master
// Description: Fonctions du temps
//------------------------------------------------------


//Retourne le temps en us depuis de debut
//Fonction a ne pas utiliser
//@param rien
//@return le temps en us
unsigned long int get_time_us(void);

//Retourne le temps en ms depuis de debut
//@param rien
//@return le temps en ms
unsigned long int get_time_ms(void);

//Incremente le temps
//Fonction appeler par interuption toutes les ms
//@param rien
//@return rien
void Time_increment(void);

//Attend un delay de 1ms
//@param rien
//@return rien
void Delay_1ms(void);

//Attend un delay de 1us
//@param rien
//@return rien
void Delay_1us(void);
