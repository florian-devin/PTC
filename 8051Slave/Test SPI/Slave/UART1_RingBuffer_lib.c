//------------------------------------------------------
// Date: 26/03/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: UART1_RingBuffer_lib.c
// Destination: 8150F020 Carte Master
// Description: Fonctions pour l'uart1 avec buffer circ
//------------------------------------------------------
//*************************************************************************************************
#include <c8051f020.h>                    // SFR declarations
#include <stdio.h>
#include <intrins.h>
#include <string.h>
#include "UART1_RingBuffer_lib.h"
 
//*************************************************************************************************
// Parametresd modifiables
//*************************************************************************************************
#define       MAX_BUFLEN1 64 // Taille des buffers de donnees

//*************************************************************************************************
// DEFINITION DES MACROS DE GESTION DE BUFFER CIRCULAIRE
//*************************************************************************************************

// Structure de gestion de buffer circulaire
	//rb_start1: pointeur sur l'adresse de debut du buffer 
	// rb_end1: pointeur sur l'adresse de fin du buffer	
	// rb_in1: pointeur sur la donnee a lire
	// rb_out1: pointeur sur la case a ecrire
		
#define RB_CREATE1(rb1, type) \
   struct { \
     type *rb_start1; \	   
     type *rb_end1; \	   
     type *rb_in1; \
	   type *rb_out1; \		
	  } rb1

//Initialisation de la structure de pointeurs 
// rb1: adresse de la structure
// start1: adresse du premier element du buffer 
// number1: nombre d'elements du buffer - 1	(le "-1" n'est a mon avis pas necessaire)
#define rb_in1IT1(rb1, start1, number1) \
         ( (rb1)->rb_in1 = (rb1)->rb_out1= (rb1)->rb_start1= start1, \
           (rb1)->rb_end1 = &(rb1)->rb_start1[number1] )

//Cette macro rend le buffer circulaire. Quand on atteint la fin, on retourne au debut
#define RB_SLOT1(rb1, slot1) \
         ( (slot1)==(rb1)->rb_end1? (rb1)->rb_start1: (slot1) )

// Test: Buffer vide? 
#define RB_EMPTY1(rb1) ( (rb1)->rb_in1==(rb1)->rb_out1 )

// Test: Buffer plein?
#define RB_FULL1(rb1)  ( RB_SLOT1(rb1, (rb1)->rb_in1+1)==(rb1)->rb_out1 )

// Incrementation du pointeur dur la case a ecrire
#define RB_PUSHADVANCE1(rb1) ( (rb1)->rb_in1= RB_SLOT1((rb1), (rb1)->rb_in1+1) )

// Incrementation du pointeur sur la case a lire
#define RB_POPADVANCE1(rb1)  ( (rb1)->rb_out1= RB_SLOT1((rb1), (rb1)->rb_out1+1) )

// Pointeur pour stocker une valeur dans le buffer
#define RB_PUSHSLOT1(rb1) ( (rb1)->rb_in1 )

// pointeur pour lire une valeur dans le buffer
#define RB_POPSLOT1(rb1)  ( (rb1)->rb_out1 )


//*************************************************************************************************


/* Transmission and Reception Data Buffers */
static char  xdata outbuf1[MAX_BUFLEN1];     /* memory for transmission ring buffer #1 (TXD) */
static char  xdata inbuf1 [MAX_BUFLEN1];     /* memory for reception ring buffer #2 (RXD) */
static  bit   TXactive1 = 0;             /* transmission status flag (off) */

/* define out1 (transmission)  and in1 (reception)  ring buffer control structures */
static RB_CREATE1(out1,unsigned char xdata);            /* static struct { ... } out1; */
static RB_CREATE1(in1, unsigned char xdata);            /* static struct { ... } in1; */

//**************************************************************************************************
//**************************************************************************************************
void UART1_ISR(void) interrupt 20 {
 
    //	static unsigned int cp_tx = 0;
    //  static unsigned int cp_rx = 0;
	
  if ((SCON1 & 0x02) == 0x02) // On peut envoyer une nouvelle donnee sur la liaison serie
  { 
  	if(!RB_EMPTY1(&out1)) {
       SBUF1 = *RB_POPSLOT1(&out1);      /* start1 transmission of next byte */
       RB_POPADVANCE1(&out1);            /* remove the sent byte from buffer */
    //			 cp_tx++;
  	}
  	else {
      TXactive1 = 0;                 /* TX finished, interface inactive */
    }
	  SCON1 &= ~(1<<1); //TI1 = 0
  }
  else // RI0 a 1 - Donc une donnee a ete reeue
  {
		if(!RB_FULL1(&in1)) {                   // si le buffer est plein, la donnee reeue est perdue
     	*RB_PUSHSLOT1(&in1) = SBUF1;        /* store new data in1 the buffer */
		  RB_PUSHADVANCE1(&in1);               /* next write location */
    //		  cp_rx++;
	 }
   SCON1 &= ~(1<<0); //RI1 = 0
  }
  SCON1 &= ~(0x02); //TI1 = 0 et RIO = 0
}
// **************************************************************************************************
// init_Serial_Buffer_uart1: Initialisation des structuresde gestion des buffers transmission et reception
// *************************************************************************************************
//**************************************************************************************************
void init_Serial_Buffer_uart1(void) {

    rb_in1IT1(&out1, outbuf1, MAX_BUFLEN1-1);           /* set up TX ring buffer */
    rb_in1IT1(&in1, inbuf1,MAX_BUFLEN1-1);             /* set up RX ring buffer */

}



unsigned char serOutchar_uart1(char c) {

  if(!RB_FULL1(&out1))  // si le buffer n'est pas plein, on place l'octet dans le buffer
  {                 
  	*RB_PUSHSLOT1(&out1) = c;               /* store data in1 the buffer */
  	RB_PUSHADVANCE1(&out1);                 /* adjust write position */

  	if(!TXactive1) {
		TXactive1 = 1;                      /* indicate ongoing transmission */
 	  SCON1 |= (1<<1);//   Placer le bit TI a 1 pour provoquer le declenchement de l'interruption
  	}
	return 0;  // operation correctement realisee 
  }
   else return 1; // operation echouee - Typiquement Buffer plein
}



char serInchar_uart1(void) {
  char c;

  if (!RB_EMPTY1(&in1))
  {                 /* wait for data */

  	c = *RB_POPSLOT1(&in1);                 /* get character off the buffer */
 	  RB_POPADVANCE1(&in1);                   /* adjust read position */
  	return c;
  }
  else return 0;
}



unsigned int serInchar_Bin_uart1(void) {
  char c;
  unsigned int return_code = 0;
	 
  if (!RB_EMPTY1(&in1))
  {                
    // un caractere au moins est dans le buffer de reception
  	c = *RB_POPSLOT1(&in1);                 /* get character off the buffer */
 	  RB_POPADVANCE1(&in1);                   /* adjust read position */
  	return 0xFF00+c;
  }
	// pas de caractere dans le buffer de reception.
  else return return_code;
}


unsigned char serOutstring_uart1(char *buf) {
  unsigned char len,code_err=0;

  for(len = 0; len < strlen(buf); len++)
     code_err +=serOutchar_uart1(buf[len]);
  return code_err;
}

