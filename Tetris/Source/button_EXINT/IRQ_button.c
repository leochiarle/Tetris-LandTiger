#include "button.h"
#include "LPC17xx.h"
#include "GLCD.h" 

extern int buttonKey1Debouncing;
extern int buttonKey2Debouncing;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	buttonKey1Debouncing = 7;

	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
	buttonKey2Debouncing = 7;
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


