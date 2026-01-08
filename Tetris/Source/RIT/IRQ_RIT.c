/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "GLCD.h" 

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int stopDownShift;
extern ActivePiece activePiece;
extern int goDown;
extern int tetrominoStopped;
extern int joystickMovement;
extern int buttonKey1Debouncing;
extern int buttonKey2Debouncing;
extern int gameOver;
extern int gamePaused;
extern int restartGame;
extern int dropToBottom;

void RIT_IRQHandler (void)
{			
	
	/* Key1 was pressed */
	if(buttonKey1Debouncing == 2){
		buttonKey1Debouncing = 1;
		if( gameOver ){
		
			restartGame++;
			
		}else{
			
			if( gamePaused ){
				gamePaused--;
			}else{
				gamePaused++;
			}
		}
	}
	
	/* Key2 was pressed */
	if(buttonKey2Debouncing == 2){
		dropToBottom++;
		buttonKey2Debouncing = 1;
	}
	
	/* Check for piece movement down */
	if( activePiece.field_end_y > 0 && !stopDownShift){
		
		goDown++;
		
	}else{
		
		tetrominoStopped++;
	
	}
	
	/* Active joystick movement detection */
	if( joystickMovement == 0 ){
		
		joystickMovement++;
	
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
