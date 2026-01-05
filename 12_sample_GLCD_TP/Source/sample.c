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
#ifndef __HEADER_FILES__
#define __HEADER_FILES__
#include "LPC17xx.h"
#include "GLCD.h"
#include "timer.h"
#include "RIT.h"
#include "joystick.h"

#include <stdio.h> /*for sprintf*/
#include <stdlib.h>
#include <time.h>

#endif


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

FieldBlock playField[FIELD_HEIGHT_BLOCKS][FIELD_WIDTH_BLOCKS];
ActivePiece activePiece;
int goDown = 0;
int stopDownShift = 0;
int dropToBottom = 0;
int tetrominoStopped = 0;
int gameScore = 0;
int highestScore = 0;
int lineCount = 0;
int gamePaused = 1;
int gameOver = 0;
int restartGame = 0;
char strHighestScore[13];
char strScore[13];
char strLineCount[13];

void * memcpy(void *, const void *, size_t);

	
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
	

void createTetromino(){
	
	//int rd_num = rand() % 7;
	int rd_num = 4;
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
	
	int aux_x = activePiece.start_x, aux_y = activePiece.start_y;
	
	activePiece.field_start_x = (FIELD_WIDTH_BLOCKS/2)-((activePiece.end_x - aux_x));
	activePiece.field_start_y = FIELD_HEIGHT_BLOCKS-1;
	activePiece.field_end_x = activePiece.field_start_x + (activePiece.end_x - aux_x);
	activePiece.field_end_y = activePiece.field_start_y - (activePiece.end_y - aux_y);
	
	stopDownShift = 0;
	
	isGameOver( &activePiece ); // Has be checked before draw the nre tetromino
	LCD_DrawTetromino( &activePiece );
}

void initStringsScore(){
	
	// Highest Score
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 20, (uint8_t *) "Highscore:", Red, White);
	 
	sprintf(strHighestScore, "%d", highestScore);
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 35, (uint8_t *) strHighestScore, Red, White);
	 
	// Game Score
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 60, (uint8_t *) "Score:", Red, White);

	sprintf(strScore, "%d", gameScore);
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 75, (uint8_t *) strScore, Red, White);
	
	// Line Count
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 100, (uint8_t *) "Line:", Red, White);

	sprintf(strLineCount, "%d", gameScore);
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 115, (uint8_t *) strLineCount, Red, White);
	
}

void initEverithing(){
	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	LCD_Clear(Black);

	joystick_init();											/* Joystick Initialization            */
  BUTTON_init();												/* BUTTON Initialization              */
	
	initStringsScore();
	
	init_timer(0, 0x4E2 ); 						/* 500us * 25MHz = 1.25*10^6 = 0x1312D0 */
	
}

void cleanPlayFied(){
	
	int x = 0, y = 0;
	
	for( ; y < FIELD_HEIGHT_BLOCKS; y++ ){
		for( ; x < FIELD_WIDTH_BLOCKS; x++ ){
			if( playField[y][x].full == 1 ){
				LCD_CleanCube( x, y );
			}
		}
		x = 0;
	}
}

void newGame(){
	
	goDown = 0;
	stopDownShift = 0;
	tetrominoStopped = 0;
	gameScore = 0;
	lineCount = 0;
	gamePaused = 1;
	gameOver = 0;
	restartGame = 0;
	
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 140, (uint8_t *) "Gameover", Black, Black); // clean text "gameover"
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 75, (uint8_t *) "00000000000", Black, Black); // clean text score point
	
	initStringsScore();
	cleanPlayFied();
	createTetromino();
	
}

int main(void)
 {
  
	initEverithing();
	
	LCD_DrawLine(MAX_X - INFO_FIELD + 1, MAX_Y - PLAY_FIELD_HEIGHT, MAX_X - INFO_FIELD + 1, MAX_Y, White);
	LCD_DrawLine(0, MAX_Y - PLAY_FIELD_HEIGHT - 1, PLAY_FIELD_WIDTH, MAX_Y - PLAY_FIELD_HEIGHT - 1, White);
	 
	
	/*	*/
	memcpy(&activePiece, &L, sizeof(Piece));
	int aux_x = activePiece.start_x, aux_y = activePiece.start_y;
	
	activePiece.field_start_x = (FIELD_WIDTH_BLOCKS/2)-((activePiece.end_x - aux_x));
	activePiece.field_start_y = FIELD_HEIGHT_BLOCKS-1;
	activePiece.field_end_x = activePiece.field_start_x + (activePiece.end_x - aux_x);
	activePiece.field_end_y = activePiece.field_start_y - (activePiece.end_y - aux_y);
	
	stopDownShift = 0;
	LCD_DrawTetromino( &activePiece );
	/*
	int i = 0;
	for( ; i < FIELD_HEIGHT_BLOCKS - 1 - 2; i++ ){
		LCD_DrawCube(0, i, Yellow);
		LCD_DrawCube(1, i, Yellow);
		LCD_DrawCube(2, i, Yellow);
		LCD_DrawCube(3, i, Yellow);
		LCD_DrawCube(4, i, Yellow);
		LCD_DrawCube(5, i, Yellow);
		LCD_DrawCube(6, i, Yellow);
		LCD_DrawCube(7, i, Yellow);
		LCD_DrawCube(8, i, Yellow);
		//LCD_DrawCube(9, i, Yellow);
	}
	*/
	//LCD_ShiftRows( 0, 1 );



	//createTetromino();
	 
	while(1){
		
		if( gameOver ){
			
			disable_timer(0);	// stop timer 0
			
			GUI_Text(MAX_X - (INFO_FIELD / 1.1), 140, (uint8_t *) "Gameover", Red, White);
			
			if( gameScore > highestScore){
				highestScore = gameScore;
			}
			
			if( restartGame ){
				newGame();
			}
			
		}else if( gamePaused ){
			
			disable_timer(0);	// stop timer 0
			
		}else{ // game not in pause
			
			enable_timer(0);
			 
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	// Joytick UP pressed 
				
				disable_timer(0);	// stop timer 0
				
				rotateTetromino( &activePiece );

				enable_timer(0);	// resume timer 0
				
			} else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && activePiece.end_x < FIELD_WIDTH_BLOCKS){	// Joytick RIGHT pressed 
				
				disable_timer(0);	// stop timer 0
				
				LCD_RightShiftTetromino( &activePiece );

				enable_timer(0);	// resume timer 0
				
			} else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && activePiece.start_x > 0){	// Joytick LEFT pressed
				
				disable_timer(0);	// stop timer 0
				
				LCD_LeftShiftTetromino( &activePiece );

				enable_timer(0);	// resume timer 0
				
			} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && activePiece.end_y > 0){	// Joytick DOWN pressed 
				
				disable_timer(0);	// stop timer 0
				
				LCD_DownShiftTetromino( &activePiece );

				enable_timer(0);	// resume timer 0
			}
			
			if( goDown ){
				
				disable_timer(0);	// stop timer 0
				
				LCD_DownShiftTetromino( &activePiece );
				goDown--;

				enable_timer(0);	// resume timer 0
			}
			
			if( dropToBottom ){
				
				disable_timer(0);	// stop timer 0

				while( possibleDownShift( &activePiece ) ){
					LCD_DownShiftTetromino( &activePiece );
				}
				
				tetrominoStopped++;
				dropToBottom--;
				
				enable_timer(0);	// resume timer 0
				
			}
			
			if( tetrominoStopped ){
				
				//if(  ){ // possible game over
					
				//}else{ // not game over
				
					disable_timer(0);	// stop timer 0
				
					shiftRowsFull();
					
					gameScore += 10;
					sprintf(strScore, "%d", gameScore);
					GUI_Text(MAX_X - (INFO_FIELD / 1.1), 75, (uint8_t *) strScore, Red, White);
					
					sprintf(strLineCount, "%d", lineCount);
					GUI_Text(MAX_X - (INFO_FIELD / 1.1), 115, (uint8_t *) strLineCount, Red, White);
					
					createTetromino();
					tetrominoStopped--;
					
					enable_timer(0);	// resume timer 0
				
				//}
				
			}
		
		}
	}
	
	
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
