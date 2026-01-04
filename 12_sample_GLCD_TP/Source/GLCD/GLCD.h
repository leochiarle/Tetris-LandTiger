/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.h
** Descriptions:		Has been tested SSD1289、ILI9320、R61505U、SSD1298、ST7781、SPFD5408B、ILI9325、ILI9328、
**						HX8346A、HX8347A
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2012-3-10
** Version:				1.3
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __GLCD_H 
#define __GLCD_H

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

/* Private define ------------------------------------------------------------*/

/* LCD Interface */
#define PIN_EN		(1 << 19)
#define PIN_LE		(1 << 20)
#define PIN_DIR		(1 << 21)
#define PIN_CS      (1 << 22)
#define PIN_RS		(1 << 23)
#define PIN_WR		(1 << 24)
#define PIN_RD		(1 << 25)   

#define LCD_EN(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_EN) : (LPC_GPIO0->FIOCLR = PIN_EN));
#define LCD_LE(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_LE) : (LPC_GPIO0->FIOCLR = PIN_LE));
#define LCD_DIR(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_DIR) : (LPC_GPIO0->FIOCLR = PIN_DIR));
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS));
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS));
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR));
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD));

/* Private define ------------------------------------------------------------*/
#define DISP_ORIENTATION  0  /* angle 0 90 */ 

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Orange         0xfc9803
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : 24位转换16位
* Input          : - red: R
*                  - green: G 
*				   - blue: B
* Output         : None
* Return         : RGB 颜色值
* Attention		 : None
*******************************************************************************/
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initialization(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

#define FIELD_HEIGHT_BLOCKS			20
#define FIELD_WIDTH_BLOCKS 			10
//#define BLOCK 								((MAX_Y-1)/HEIGHT)
#define INFO_FIELD 							90 //(MAX_X * 1/2)
#define PADDING 								0 // FIX: il padding deve essere lo scarto per avere la size giusta di 20x10 del play field
#define PLAY_FIELD_WIDTH 				MAX_X - INFO_FIELD - PADDING
#define BLOCK 									15 //PLAY_FIELD_WIDTH/WIDTH // 16 // FIX: non funziona la divisione
#define PLAY_FIELD_HEIGHT				(BLOCK * FIELD_HEIGHT_BLOCKS) - PADDING


typedef struct {
	
	const int I[4][4];
	const int O[4][4];
	const int T[4][4];
	const int J[4][4];
	const int L[4][4];
	const int S[4][4];
	const int Z[4][4];

} Tetrominos;

typedef struct {
	
	int full;
	uint16_t color;

} FieldBlock;

typedef struct {
	
	uint16_t color;
	int tetromino[4][4];
	int start_x;
	int start_y;
	int end_x;
	int end_y;

} Piece;

typedef struct {
	
	uint16_t color;
	int tetromino[4][4];
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	int field_start_x;
	int field_start_y;
	int field_end_x;
	int field_end_y;

} ActivePiece;

void LCD_DrawNewTetromino( ActivePiece* p );

void LCD_LeftShiftTetromino( ActivePiece* p, uint16_t shift );
void LCD_RightShiftTetromino( ActivePiece* p, uint16_t shift );
void LCD_DownShiftTetromino( ActivePiece* p, uint16_t shift );
void LCD_ShiftRows( uint16_t fromRow, uint16_t toRow );
int possibleLeftShift( ActivePiece* p, uint16_t shift );
int possibleRightShift( ActivePiece* p, uint16_t shift );
int possibleDownShift( ActivePiece* p, uint16_t shift );
void shiftRowsFull();
int firstRowEmpty();

/* rewatch and fix */
void rotateTetromino( ActivePiece* p );

void LCD_DrawCube( uint16_t x, uint16_t y, uint16_t bkColor );
void LCD_ClearCube ( uint16_t x, uint16_t y );
void LCD_ClearRow ( uint16_t y );
int fullRow( uint16_t row );

