/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD.h"
#include "timer.h"
#include "RIT.h"
#include "joystick.h"

#include <stdio.h> /*for sprintf*/
#include <stdlib.h>
#include <time.h>


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int playField[FIELD_WIDTH_BLOCKS][FIELD_HEIGHT_BLOCKS];
ActivePiece activePiece;
int goDown = 0;
int stopDownShift = 0;
int tetrominoStopped = 0;

void * memcpy(void *, const void *, size_t);
void free(void *);


	Tetrominos tetrominos = {
		
		.I = {{0, 0, 0, 0}, 
					{1, 1, 1, 1},
					{0, 0, 0, 0},
					{0, 0, 0, 0}},
		
		.O = {{0, 0, 0, 0}, 
					{0, 1, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0}},
		
		.T = {{0, 0, 0, 0}, 
					{1, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0}},
		
		.J = {{0, 0, 1, 0}, 
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0}},
		
		.L = {{0, 1, 0, 0}, 
					{0, 1, 0, 0}, 
					{0, 1, 1, 0},
					{0, 0, 0, 0}},
		
		.S = {{0, 0, 0, 0}, 
					{0, 1, 1, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0}},
		
		.Z = {{0, 0, 0, 0}, 
					{1, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0}}
	}; // TODO: aggiungere le rotazioni (?)
	
	
	Piece I = {
		.color = Cyan,
		.tetromino = {{0, 0, 0, 0}, 
									{1, 1, 1, 1},
									{0, 0, 0, 0},
									{0, 0, 0, 0}},
		.start_x = 0,
		.start_y = 1,
		.end_x = 3,
		.end_y = 1
	};
	
	Piece O = {
		.color = Yellow,
		.tetromino = {{0, 0, 0, 0}, 
									{0, 1, 1, 0},
									{0, 1, 1, 0},
									{0, 0, 0, 0}},
		.start_x = 1,
		.start_y = 1,
		.end_x = 2,
		.end_y = 2
	};
	
	Piece T = {
		.color = Magenta,
		.tetromino = {{0, 0, 0, 0}, 
									{1, 1, 1, 0},
									{0, 1, 0, 0},
									{0, 0, 0, 0}},
		.start_x = 0,
		.start_y = 1,
		.end_x = 2,
		.end_y = 2
	};
	
	Piece J = {
		.color = Blue,
		.tetromino = {{0, 0, 1, 0}, 
									{0, 0, 1, 0},
									{0, 1, 1, 0},
									{0, 0, 0, 0}},
		.start_x = 1,
		.start_y = 0,
		.end_x = 2,
		.end_y = 2
	};	
	
	Piece L = {
		.color = Orange,
		.tetromino = {{0, 1, 0, 0}, 
									{0, 1, 0, 0}, 
									{0, 1, 1, 0},
									{0, 0, 0, 0}},
		.start_x = 1,
		.start_y = 0,
		.end_x = 2,
		.end_y = 2
	};
	
	Piece S = {
		.color = Green,
		.tetromino = {{0, 0, 0, 0}, 
									{0, 1, 1, 0},
									{1, 1, 0, 0},
									{0, 0, 0, 0}},
		.start_x = 0,
		.start_y = 1,
		.end_x = 2,
		.end_y = 2
	};
	
	Piece Z = {
		.color = Red,
		.tetromino = {{0, 0, 0, 0}, 
									{1, 1, 0, 0},
									{0, 1, 1, 0},
									{0, 0, 0, 0}},
		.start_x = 0,
		.start_y = 1,
		.end_x = 2,
		.end_y = 2
	};
	
	Piece TEST = {
		.color = Red,
		.tetromino = {{1, 1, 1, 1}, 
									{1, 1, 1, 1},
									{1, 1, 1, 1},
									{1, 1, 1, 1}},
		.start_x = 0,
		.start_y = 0,
		.end_x = 4,
		.end_y = 4
	};
	

void createTetromino(){
	
	int max = 6, min = 0;
	int rd_num = rand() % (max - min + 1) + min;
	
	switch(rd_num){
		case 0:
			memcpy(&activePiece, &I, sizeof(Piece));
			break;
		
		case 1:
			memcpy(&activePiece, &O, sizeof(Piece));
			break;
		
		case 2:
			memcpy(&activePiece, &T, sizeof(Piece));
			break;
		
		case 3:
			memcpy(&activePiece, &J, sizeof(Piece));
			break;
		
		case 4:
			memcpy(&activePiece, &L, sizeof(Piece));
			break;
		
		case 5:
			memcpy(&activePiece, &S, sizeof(Piece));
			break;
		
		case 6:
			memcpy(&activePiece, &Z, sizeof(Piece));
			break;

	}
	stopDownShift = 0;
	LCD_DrawNewTetromino( &activePiece );
}

void initEverithing(){
	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	LCD_Clear(Black);

	joystick_init();											/* Joystick Initialization            */
	
}


int main(void)
 {
  
	initEverithing();
	
	LCD_DrawLine(MAX_X - INFO_FIELD + 1, MAX_Y - PLAY_FIELD_HEIGHT, MAX_X - INFO_FIELD + 1, MAX_Y, White);
	LCD_DrawLine(0, MAX_Y - PLAY_FIELD_HEIGHT - 1, PLAY_FIELD_WIDTH, MAX_Y - PLAY_FIELD_HEIGHT - 1, White);
	
	
	memcpy(&activePiece, &L, sizeof(Piece));
	LCD_DrawNewTetromino( &activePiece );
	//createTetromino();
	
	init_timer(0, 0x4E2 ); 						/* 500us * 25MHz = 1.25*10^6 = 0x1312D0 */
	enable_timer(0);	
	 
	while(1){
		
		if( tetrominoStopped ){
			
			disable_timer(0);	// stop timer 0
			
			createTetromino();
			tetrominoStopped--;
			
			enable_timer(0);	// resume timer 0
			
		}
		 
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	// Joytick UP pressed 
			
			disable_timer(0);	// stop timer 0
			
			rotateTetromino( &activePiece );

			enable_timer(0);	// resume timer 0
			
		} else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && activePiece.end_x < FIELD_WIDTH_BLOCKS){	// Joytick RIGHT pressed 
			
			disable_timer(0);	// stop timer 0
			
			LCD_RightShiftTetromino( &activePiece, 1 );

			enable_timer(0);	// resume timer 0
			
		} else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && activePiece.start_x > 0){	// Joytick LEFT pressed
			
			disable_timer(0);	// stop timer 0
			
			LCD_LeftShiftTetromino( &activePiece, 1 );

			enable_timer(0);	// resume timer 0
			
		} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && activePiece.end_y > 0){	// Joytick DOWN pressed 
			
			disable_timer(0);	// stop timer 0
			
			LCD_DownShiftTetromino( &activePiece, 1 );

			enable_timer(0);	// resume timer 0
		}
		
		if( goDown ){
			
			disable_timer(0);	// stop timer 0
			
			LCD_DownShiftTetromino( &activePiece, 1 );
			goDown--;

			enable_timer(0);	// resume timer 0
		}
		
	}
	
	/* Draw all and clear all
	i = 0;
	for(; i < 10; i++){
		for(; j < 20; j++){
			LCD_DrawCube(i, j, Red);
			//LCD_DrawCube(i, j+1, Blue);
		}
		j = 0;
	}
	
	i = 0;
	for(; i < 10; i++){
		for(; j < 20; j++){
			LCD_ClearCube(i, j);
		}
		j = 0;
	}
	*/
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	//enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	SCB->SCR |= 0x2;											/* set SLEEPONEXIT */
	
	__ASM("wfi");

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
