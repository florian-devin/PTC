#include "PTC_Aquisition.h"
#include "stm32f4xx_hal.h"

#define MAX_BUFLEN_DATA_IN 256

// Structure de gestion de buffer circulaire
	//rb_start: pointeur sur l'adresse de debut du buffer 
	// rb_end: pointeur sur l'adresse de fin du buffer	
	// rb_in: pointeur sur la donnee e lire
	// rb_out: pointeur sur la case e ecrire
		
#define RB_CREATE(rb, type) \
   struct { \
     type *rb_start; \
     type *rb_end; \
     type *rb_in; \
	   type *rb_out; \
	  } rb

//Initialisation de la structure de pointeurs 
// rb: adresse de la structure
// start: adresse du premier element du buffer 
// number: nombre d'elements du buffer - 1	(le "-1" n'est e mon avis pas necessaire)
#define RB_INIT(rb, start, number) \
         ( (rb)->rb_in = (rb)->rb_out= (rb)->rb_start= start, \
           (rb)->rb_end = &(rb)->rb_start[number] )

//Cette macro rend le buffer circulaire. Quand on atteint la fin, on retourne au debut
#define RB_SLOT(rb, slot) \
         ( (slot)==(rb)->rb_end? (rb)->rb_start: (slot) )

// Test: Buffer vide? 
#define RB_EMPTY(rb) ( (rb)->rb_in==(rb)->rb_out )

// Test: Buffer plein?
#define RB_FULL(rb)  ( RB_SLOT(rb, (rb)->rb_in+1)==(rb)->rb_out )

// Incrementation du pointeur dur la case a ecrire
#define RB_PUSHADVANCE(rb) ( (rb)->rb_in= RB_SLOT((rb), (rb)->rb_in+1) )

// Incrementation du pointeur sur la case a lire
#define RB_POPADVANCE(rb)  ( (rb)->rb_out= RB_SLOT((rb), (rb)->rb_out+1) )

// Pointeur pour stocker une valeur dans le buffer
#define RB_PUSHSLOT(rb) ( (rb)->rb_in )

// pointeur pour lire une valeur dans le buffer
#define RB_POPSLOT(rb)  ( (rb)->rb_out )


//Ring Buffer
static unsigned char Data_IN_buf[MAX_BUFLEN_DATA_IN];     // memory for transmission ring buffer #1 (Data_IN)

/* define out (transmission)  and in (reception)  ring buffer control structures */
static RB_CREATE(Data_IN,unsigned char);


//Initialise le RingBuffer pour les echantillons d'entree
void init_RingBuffer_Data_IN(void) {
    RB_INIT(&Data_IN, Data_IN_buf, MAX_BUFLEN_DATA_IN-1);  
}

//Ajoute une valeur au RingBuffer des echantillons (ecrase la premiere valeur si il est plein)
//@param echantillon echantillon a placer dans le ring buffer
void put_RingBuffer_Data_IN(uint8_t echantillon) {
    *RB_PUSHSLOT(&Data_IN);
    RB_PUSHADVANCE(&Data_IN);
}


//Place dans Donnes l'ensemble du buffer
//@param Donnes pointeur sur le tableau de donnes a recuperer
void getFull_RingBuffer_Data_IN (uint8_t *Donnes) {
    uint8_t i;
    for (i = 0; i < MAX_BUFLEN_DATA_IN; i++) {
        RB_PUSHADVANCE(&Data_IN);
        Donnes[i] = *RB_POPSLOT(&Data_IN);
    }
}

uint8_t isFull_RingBuffer_Data_IN (void) {
    if (RB_FULL(&Data_IN))
        return 1;
    return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

extern ADC_HandleTypeDef hadc1;


//Lance une aquisition sur le ADC1
void MY_Aquisition(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,10);
    put_RingBuffer_Data_IN(HAL_ADC_GetValue(&hadc1));
    HAL_ADC_Stop(&hadc1);
}
