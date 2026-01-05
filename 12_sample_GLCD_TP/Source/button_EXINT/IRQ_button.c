#include "button.h"
#include "LPC17xx.h"
#include "GLCD.h" 

extern int gamePaused;
extern int gameOver;
extern ActivePiece activePiece;
extern int tetrominoStopped;
extern int restartGame;
extern int dropToBottom;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	
	if( gameOver ){
		
		restartGame++;
		
	}else{
		
		if( gamePaused ){
			gamePaused--;
		}else{
			gamePaused++;
		}
		
	}
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
	dropToBottom++;
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


