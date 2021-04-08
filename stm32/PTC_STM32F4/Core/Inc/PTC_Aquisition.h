
#include "main.h"

//Initialise le RingBuffer pour les echantillons d'entree
void init_RingBuffer_Data_IN(void);

//Ajoute une valeur au RingBuffer des echantillons (ecrase la premiere valeur si il est plein)
//@param echantillon echantillon a placer dans le ring buffer
void put_RingBuffer_Data_IN(uint8_t echantillon);

//Place dans Donnes l'ensemble du buffer
//@param Donnes pointeur sur le tableau de donnes a recuperer
void getFull_RingBuffer_Data_IN (uint8_t *Donnes);

//Indique que le buffer circulaire est plein
//@return 1 si le buffer est plein 0 sinon
uint8_t isFull_RingBuffer_Data_IN (void);



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


//Lance une aquisition sur le ADC1
void MY_Aquisition(void);
