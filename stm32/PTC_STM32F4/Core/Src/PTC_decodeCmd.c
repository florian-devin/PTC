//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Pr�nom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_decodeCmd.c
// Destination: STM32
// Description: Fonctions qui permette de cecomposer les commandes du PC de commande
//------------------------------------------------------
#include <string.h>
#include <math.h>
#include "main.h"
#include "PTC_geter_cmd.h"
#include "PTC_convertion.h"
#include "PTC_decodeCmd.h"

extern char Code_frequence;
extern int  Timer_freq;
extern unsigned int Duree_son;
extern unsigned int Duree_silence;
extern char Nombre_bips;
extern char Sound_Loop_Enable;
extern unsigned long Temp_init;

void Cmd_epreuve_SD_stm32(const char *Pchaine_courante) { //Commande generation de signaux sonore
	char str_param[8] = {0};
	char num_param = 1;
	Code_frequence = 6;
	Duree_son = 25*10;
	Duree_silence = 50*10;
	Nombre_bips = 3;
	do {
		get_param(Pchaine_courante,num_param,str_param);
		if (strlen(str_param) != 0){
			char str_value[4] = {0};
			char str_name[2]  = {0};
			num_param++;
			get_complex_param(str_param,str_name,str_value);
			if (strcmp(str_name, "F")==0){
				Code_frequence = my_atoi(str_value);
			}
			else if  (strcmp(str_name, "P")==0){
				Duree_son = my_atoi(str_value)*10;
			}
			else if  (strcmp(str_name, "W")==0){
				Duree_silence = my_atoi(str_value)*10;
			}
			else if  (strcmp(str_name, "B")==0){
				Nombre_bips = my_atoi(str_value);
			}
			else {
				return;
			}
		} 
	} while (strlen(str_param) != 0);
	Timer_freq = (int)(5390 * exp(-0.099*Code_frequence));
	Temp_init = HAL_GetTick();
	Sound_Loop_Enable = 1;
}

