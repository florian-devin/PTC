//------------------------------------------------------
// Date: 26/03/2021
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: UART0_RingBuffer_lib.c
// Destination: 8150F020 Carte Master
// Description: Fonctions pour l'uart0 avec buffer circ
//------------------------------------------------------
//*************************************************************************************************
#include <c8051f020.h>                    // SFR declarations
#include <stdio.h>
#include <intrins.h>
#include <string.h>
#include "UART0_RingBuffer_lib.h"

//*************************************************************************************************
// Parametresd modifiables
//*************************************************************************************************
#define       MAX_BUFLEN 64 // Taille des buffers de donnees

//*************************************************************************************************
// DEFINITION DES MACROS DE GESTION DE BUFFER CIRCULAIRE
//*************************************************************************************************

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

// Incrementation du pointeur dur la case e ecrire
#define RB_PUSHADVANCE(rb) ( (rb)->rb_in= RB_SLOT((rb), (rb)->rb_in+1) )

// Incrementation du pointeur sur la case e lire
#define RB_POPADVANCE(rb)  ( (rb)->rb_out= RB_SLOT((rb), (rb)->rb_out+1) )

// Pointeur pour stocker une valeur dans le buffer
#define RB_PUSHSLOT(rb) ( (rb)->rb_in )

// pointeur pour lire une valeur dans le buffer
#define RB_POPSLOT(rb)  ( (rb)->rb_out )


//*************************************************************************************************


/* Transmission and Reception Data Buffers */
static char  xdata outbuf[MAX_BUFLEN];     /* memory for transmission ring buffer #1 (TXD) */
static char  xdata inbuf [MAX_BUFLEN];     /* memory for reception ring buffer #2 (RXD) */
static  bit   TXactive = 0;             /* transmission status flag (off) */

/* define out (transmission)  and in (reception)  ring buffer control structures */
static RB_CREATE(out,unsigned char xdata);            /* static struct { ... } out; */
static RB_CREATE(in, unsigned char xdata);            /* static struct { ... } in; */

//**************************************************************************************************
//**************************************************************************************************
void UART0_ISR(void) interrupt 4 {
 
//	static unsigned int cp_tx = 0;
//  static unsigned int cp_rx = 0;
	
  if (TI0==1) // On peut envoyer une nouvelle donnee sur la liaison serie
  { 
  	if(!RB_EMPTY(&out)) {
       SBUF0 = *RB_POPSLOT(&out);      /* start transmission of next byte */
       RB_POPADVANCE(&out);            /* remove the sent byte from buffer */
//			 cp_tx++;
  	}
  	else TXactive = 0;                 /* TX finished, interface inactive */
	TI0 = 0;
  }
  else // RI0 e 1 - Donc une donnee a ete reeue
  {
		if(!RB_FULL(&in)) {                   // si le buffer est plein, la donnee reeue est perdue
     	*RB_PUSHSLOT(&in) = SBUF0;        /* store new data in the buffer */
		  RB_PUSHADVANCE(&in);               /* next write location */
//		  cp_rx++;
	 }
   RI0 = 0;
  }
}
// **************************************************************************************************
// init_Serial_Buffer: Initialisation des structuresde gestion des buffers transmission et reception
// *************************************************************************************************
//**************************************************************************************************
void init_Serial_Buffer(void) {

    RB_INIT(&out, outbuf, MAX_BUFLEN-1);           /* set up TX ring buffer */
    RB_INIT(&in, inbuf,MAX_BUFLEN-1);             /* set up RX ring buffer */

}



unsigned char serOutchar(char c) {

  if(!RB_FULL(&out))  // si le buffer n'est pas plein, on place l'octet dans le buffer
  {                 
  	*RB_PUSHSLOT(&out) = c;               /* store data in the buffer */
  	RB_PUSHADVANCE(&out);                 /* adjust write position */

  	if(!TXactive) {
		TXactive = 1;                      /* indicate ongoing transmission */
 	  TI0 = 1;//   Placer le bit TI e 1 pour provoquer le declenchement de l'interruption
  	}
	return 0;  // operation correctement realisee 
  }
   else return 1; // operation echouee - Typiquement Buffer plein
}



char serInchar(void) {
char c;

  if (!RB_EMPTY(&in))
  {                 /* wait for data */

  	c = *RB_POPSLOT(&in);                 /* get character off the buffer */
 	  RB_POPADVANCE(&in);                   /* adjust read position */
  	return c;
  }
  else return 0;
}



unsigned int serInchar_Bin(void) {
  char c;
  unsigned int return_code = 0;
	 
  if (!RB_EMPTY(&in))
  {                
    // un caractere au moins est dans le buffer de reception
  	c = *RB_POPSLOT(&in);                 /* get character off the buffer */
 	  RB_POPADVANCE(&in);                   /* adjust read position */
  	return 0xFF00+c;
  }
	// pas de caractere dans le buffer de reception.
  else return return_code;
}


unsigned char serOutstring(char *buf) {
  unsigned char len,code_err=0;

  for(len = 0; len < strlen(buf); len++)
     code_err +=serOutchar(buf[len]);
  return code_err;
}
