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
int joystickMovement = 0;
int buttonKey1Debouncing = 1;
int buttonKey2Debouncing = 0;
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
	
	int rd_num = rand() % 7;
	//int rd_num = 4;
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
	
	isGameOver( &activePiece ); // Has to be checked before draw the nre tetromino
	
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
	
	init_RIT(0x004C4B40 * 2);									/* RIT Initialization 50 msec * n  */
	//init_RIT(0x004C4B40 / 10);	
	enable_RIT();
	
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
	buttonKey1Debouncing = 1;
	gameOver = 0;
	restartGame = 0;
	
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 140, (uint8_t *) "Gameover", Black, Black); // clean text "gameover"
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 75, (uint8_t *) "00000000000", Black, Black); // clean text score point
	GUI_Text(MAX_X - (INFO_FIELD / 1.1), 115, (uint8_t *) "00000000000", Black, Black); // clean text score point
	
	initStringsScore();
	cleanPlayFied();
	createTetromino();
	
}

int main(void)
 {
  
	initEverithing();
	
	LCD_DrawLine(MAX_X - INFO_FIELD + 1, MAX_Y - PLAY_FIELD_HEIGHT, MAX_X - INFO_FIELD + 1, MAX_Y, White);
	LCD_DrawLine(0, MAX_Y - PLAY_FIELD_HEIGHT - 1, PLAY_FIELD_WIDTH, MAX_Y - PLAY_FIELD_HEIGHT - 1, White);
	 
	createTetromino();
	 
	while(1){
		
		if( gameOver ){
			
			GUI_Text(MAX_X - (INFO_FIELD / 1.1), 140, (uint8_t *) "Gameover", Red, White);
			
			if( gameScore > highestScore){
				highestScore = gameScore;
			}
			
			if( restartGame ){
				newGame();
			}
			
		}else if( gamePaused && buttonKey1Debouncing == 1 ){
			
			GUI_Text(MAX_X - (INFO_FIELD / 1.1), 140, (uint8_t *) "Paused", Red, White); // clean text "Paused"
			
		}else{ // game not in pause
			
			GUI_Text(MAX_X - (INFO_FIELD / 1.1), 140, (uint8_t *) "Paused", Black, Black);
			 
			if( joystickMovement >= 3 ){
				
				if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	// Joytick UP pressed 
					
					rotateTetromino( &activePiece );
					joystickMovement = 0;
					
				} else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 ){	// Joytick RIGHT pressed
					
					LCD_RightShiftTetromino( &activePiece );
					joystickMovement = 0;
					
				} else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 ){	// Joytick LEFT pressed
					
					LCD_LeftShiftTetromino( &activePiece );
					joystickMovement = 0;
					
				} else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 ){	// Joytick DOWN pressed 
					
					LCD_DownShiftTetromino( &activePiece );
					joystickMovement = 0;
				}
				
			}
			
			if( goDown >= 20 ){
				
				LCD_DownShiftTetromino( &activePiece );
				goDown = 0;
				
			}
			
			if( dropToBottom && buttonKey2Debouncing == 1 ){

				while( possibleDownShift( &activePiece ) ){
					LCD_DownShiftTetromino( &activePiece );
				}
				
				tetrominoStopped++;
				dropToBottom--;
				
			}
			
			if( tetrominoStopped ){
			
				shiftRowsFull();
				
				gameScore += 10;
				sprintf(strScore, "%d", gameScore);
				GUI_Text(MAX_X - (INFO_FIELD / 1.1), 75, (uint8_t *) strScore, Red, White);
				
				sprintf(strLineCount, "%d", lineCount);
				GUI_Text(MAX_X - (INFO_FIELD / 1.1), 115, (uint8_t *) strLineCount, Red, White);
				
				createTetromino();
				tetrominoStopped = 0;
				
			}
		}
	}
	
	__ASM("wfi");

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
