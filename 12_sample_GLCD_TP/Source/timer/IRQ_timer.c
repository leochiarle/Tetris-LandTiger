/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "GLCD.h" 
#include <stdio.h> /*for sprintf*/

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/



void TIMER0_IRQHandler (void)
{
  //LPC_TIM0->IR = 1;			/* clear interrupt flag */
  LPC_TIM0->IR = 0x3F;			/* clear interrupt flag */
	/*
	int i = pos_y;
	
	for(; i > pos_y - 4; i--){
		LCD_ClearRow(i);
		LCD_DrawTetromino( 3, pos_y - 1, O);
	}
	
	pos_y--;
	*/
	
	// i valori vanno aggiornati nel timer perche' il timer e' indipendente e non aspetta la fine delle funzioni chiamate
	if(real_y > 2 ){ // TODO: && playField[pos_y+4][pos_x] == 0
		
		LCD_DownShiftTetromino(real_x, real_y, 1);
		real_y--;
		
		//LCD_LeftShiftTetromino(real_x, real_y, 1);
		//real_x--;
		
		//LCD_RightShiftTetromino(real_x, real_y, 1);
		//real_x++;
	}else{
		createTetromino();
		// senza questi il timer spawna pezzi sempre all'inizio
		real_x = 3;
		real_y = HEIGHT-1;
	}

  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
