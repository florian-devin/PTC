//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: main.c
// Destination: 8150F020 Carte Master
// Description: Fonctions main
//------------------------------------------------------

#include "c8051F020_SFR16.h"
#include "c8051F020.h"
#include "config_globale.h"
#include "UART0_RingBuffer_lib.h"
#include "UART1_RingBuffer_lib.h"
#include "SPI_RingBuffer_Master.h"
#include "PTC_accuseDeReception.h"
#include "PTC_convertion.h"
#include "PTC_geter_cmd.h"
#include "PTC_strOperateurs.h"
#include "PTC_timer.h"
#include "PTC_UART.h"
#include "PTC_deplacement.h"
#include "PTC_math.h"
#include "PTC_detection.h"
#include "PTC_telemetre.h"
#include "PTC_servoMoteurHorizontal.h"
#include "PTC_decodeCmd.h"
#include <math.h>

#define POURCENTAGE_ERREUR_TICK 10 //pourcentage d'erreur accepter sur le dif des encodeurs

//TODO : gerer la reception d'info de la part de la carte Slave 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------variables globales
//En lien avec SPI
char chaine_courante_SPI[64] = {0};//chaine total qui vas contenir le mot recu 
//En lien avec l'UART0
char chaine_courante[64] = {0};//chaine total qui vas contenir le mot recu

//En lien avec le comportement du robot
int epreuve_enable 		= 0 ; //Flag qui passe a 1 si l'utilisateur a donné le debut de l'epreuve
int vitesse_par_default = 20; //vitesse du robot par default (modifie par la cmd TV vitesse)

//En lien avec le cervo-moteur Horizontal
unsigned char 	temp_servo_H 				= 0; //temp qu'il reste au cervo H pour rejoindre sa position en ms
char 			flag_print_arrive_servo_H 	= 0; //passe a 1 si on doit indiquer l'arrive du cervo
unsigned long 	last_time_capture_servo_H 	= 0; //dernier temp capture en ms

//En lien avec la position actuelle du robot
int x_robot 	= 0; //position actualle en cm
int y_robot 	= 0; //position actualle en cm
int angle_robot = 0; //angle du robot en deg

//En lien avec la fonctionnalite FCT_02 (rejoindre des coordonnees)
char state_go_coordinates 	= 0; //machine d'etat pour la fonction go_coordinates_without_obstacles()
int go_coordinates_x, go_coordinates_y, go_coordinates_angle;//pour retenir les pos demander par l'utilisateur

//En lien avec le telemetre
sbit 	commandCapture 	= P3^3; // Commande la capture du timer2
int 	measureCycle 	= 1	  ; // Compte le nombre d'overflow du Timer 2
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//En lien avec le temp
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//headers
//void main();
void setup();
void startup();
void loop();
void decodage_commande(char *Pchaine_courante);
void envoie_info_confirmation(void);
void actu_coord_robot(void);
//ceci est un com
//---------------------------------------------Fct pour le debug
void my_println(const char *str){
	serOutstring(str);
	serOutstring("\r\n");
}
void my_print(const char *str){
	serOutstring(str);
}
//-----------------------------------------------------------------------------


void main(){
    setup();
    startup();
    while(1) {
        loop();
    }
}

void setup(){
  Init_Device(); //fonction de config_globale.c
  Init_Robot(); //rst de la carte serial etc...
}


void startup(){
	unsigned char temp_init_cervo = CDE_Servo_H(0); //positionnement du cervo a 0deg
	char c;
	Delay(temp_init_cervo*10); 
	serOutstring("go\r\n");
	while(1)
		serOutchar_SPI('\r');
	while(c=serInchar_SPI() == 0);
	serOutstring("Slave Ready\r\n");
	RAZ_str(chaine_courante_SPI);
	
}
	
void loop() {
	if (Rx_chaine(chaine_courante) == 1) {
		decodage_commande(chaine_courante);
	}
	if (state_go_coordinates > 0) {
		go_coordinates_without_obstacles(go_coordinates_x, go_coordinates_y, go_coordinates_angle);
	}
	envoie_info_confirmation();

}

void decodage_commande(char *Pchaine_courante){ //fonction qui decode les commades et les applique
	char commande[7] = {0};

	get_commande(Pchaine_courante,commande);
	if (my_strcmp(commande,"D"))
		Cmd_epreuve_D(Pchaine_courante);
	else if (my_strcmp(commande,"E"))
		Cmd_epreuve_E(Pchaine_courante);
	else if (epreuve_enable > 0) {
		if (my_strcmp(commande,"A")) //Avancer
			Cmd_epreuve_A(Pchaine_courante);
		else if (my_strcmp(commande,"B"))//Reculer
			Cmd_epreuve_B(Pchaine_courante);
		else if (my_strcmp(commande,"RG")) //Rotation gauche
			Cmd_epreuve_RG();
		else if (my_strcmp(commande,"RD")) //Rotation droite
			Cmd_epreuve_RD();
		else if (my_strcmp(commande,"RC"))  //Rotation complete
			Cmd_epreuve_RC(Pchaine_courante);
		else if (my_strcmp(commande,"RA")) //Rotation d'un certain angle
			Cmd_epreuve_RA(Pchaine_courante);
		else if (my_strcmp(commande,"S")) //Stop
			Cmd_epreuve_S();
		else if (my_strcmp(commande,"G")) //rejoit des coordonnee
			Cmd_epreuve_G(Pchaine_courante);
		else if (my_strcmp(commande,"TV")) 
			Cmd_epreuve_TV(Pchaine_courante);
		else if (my_strcmp(commande,"CS")) 
			Cmd_epreuve_CS(Pchaine_courante);
		else if (my_strcmp(commande,"L"))
			Cmd_epreuve_L(Pchaine_courante);
		else if (my_strcmp(commande,"LS"))
			Cmd_epreuve_LS(Pchaine_courante); 
		else if (my_strcmp(commande,"MI"))
			Cmd_epreuve_MI(Pchaine_courante);
		else if (my_strcmp(commande,"ME"))
			Cmd_epreuve_ME(Pchaine_courante);
		else if (my_strcmp(commande,"MOU"))
			Cmd_epreuve_MOU(Pchaine_courante);
		else if (my_strcmp(commande,"MOB"))
			Cmd_epreuve_MOB(Pchaine_courante);
		else if (my_strcmp(commande,"MOS"))
			Cmd_epreuve_MOS(Pchaine_courante);
		else if (my_strcmp(commande,"ASS"))
			Cmd_epreuve_ASS(Pchaine_courante);
		else if (my_strcmp(commande,"SD"))
			Cmd_epreuve_SD(Pchaine_courante);
		else if (my_strcmp(commande,"SPI")) { //Pour le test de la liaison SPI
			serOutstring_SPI(Pchaine_courante);
			AR_cmd_correcte();
		} 
		else 
			AR_cmd_incorrecte();
	}
	else 
		AR_cmd_incorrecte();
	RAZ_str(Pchaine_courante);
}
 
void envoie_info_confirmation(void){
	if (flag_print_arrive_servo_H == 1) {
		if(temp_servo_H - (get_time_ms() - last_time_capture_servo_H) <= 0) {
			serOutstring("AS H");
			flag_print_arrive_servo_H = 0;
		}
		else {
			temp_servo_H -= (get_time_ms() - last_time_capture_servo_H);
			last_time_capture_servo_H = get_time_ms();
		}
	}
}



void Interrupt_Time(void) interrupt 16 {//interruption declancher par l'overflow du Timer 4 (toutes les ms)
	T4CON &= ~(1<<7); //interrupt flag
    Time_increment();
}

//Interrupt du timer3 pour declancher une com SPI
void Timer3_ISR(void) interrupt 14 {
    TMR3CN &= ~(1<<7); //flag 
    SPIF = 1; //declanchement de l'ISR de SPI pour envoyer un caractere si il y en a dans le buffer 
}


/*
Interruption genere par INT6 permettant de mesurer le temps a l'etat haut du signal Echo
*/
void int6 (void) interrupt 18
{
	if ((P3IF & 0x04) == 0x04)
	{
		int* bool_echo1;
		bool_echo1 = get_bool_echo1_AV();
		TL2 = 0x00;
		TH2 = 0x00;
		P3IF |= 0x04;
		*bool_echo1 = 1;
		P3IF &= 0xBB;
	} else 
	{
		float* T;
		int* bool_echo2;
		commandCapture = 1;
		commandCapture = 0;
		T = get_T_AV(); 
		bool_echo2 = get_bool_echo2_AV();
		*T = RCAP2/22.1184;
		*bool_echo2 = 1;

		P3IF &= 0xBF;
		EXF2 = 0;
	}
}

/*
Interruprion overflow Timer2
Permet de rappeler la commande de detection d'obstacle si temps d'acquisition trop long
*/
void intT2 () interrupt 5
{
	int *bool_trig_AV;
	bool_trig_AV = get_bool_trig_AV();
	measureCycle += 1;
	if (measureCycle == 21) // 60.9 ms
	{
		*bool_trig_AV = 1;
		measureCycle = 1;
	}
	TF2 = 0;
}
