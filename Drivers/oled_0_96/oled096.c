/*
 * oled096.c
 *
 *  Created on: Sep 18, 2021
 *      Author: Maxim Hodnenko
 */


#include "oled096.h"



/* Global variables */


OLED_HandleTypeDef OLED096;


/* Global variables END*/

uint8_t LCD_Buffer[OLED_WIDTH * OLED_HEIGHT / 8];
uint8_t temp_char[7] = {0,0,0,0,0,0,0}; 											// ?

unsigned char LCD_X,LCD_Y;  														//Cursor coordinates

static const uint8_t Font_6x6_RuEng[0x320] =
{
    //-- 32 English + symbols
	0x00, 0x00, 0x00, 0x00, 0x00,// 20 space
	0x00, 0x00, 0x5F, 0x00, 0x00,// 21 !
	0x00, 0x07, 0x00, 0x07, 0x00,// 22 "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// 23 #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// 24 $
	0x23, 0x13, 0x08, 0x64, 0x62,// 25 %
	0x36, 0x49, 0x55, 0x22, 0x50,// 26 &
	0x00, 0x05, 0x03, 0x00, 0x00,// 27 '
	0x00, 0x1C, 0x22, 0x41, 0x00,// 28 (
	0x00, 0x41, 0x22, 0x1C, 0x00,// 29 )
	0x14, 0x08, 0x3E, 0x08, 0x14,// 2a *
	0x08, 0x08, 0x3E, 0x08, 0x08,// 2b +
	0x00, 0x50, 0x30, 0x00, 0x00,// 2c ,
	0x08, 0x08, 0x08, 0x08, 0x08,// 2d -
	0x00, 0x60, 0x60, 0x00, 0x00,// 2e .
	0x20, 0x10, 0x08, 0x04, 0x02,// 2f /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 30 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 31 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 32 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 33 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 34 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 35 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 36 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 37 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 38 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 39 9
	0x00, 0x36, 0x36, 0x00, 0x00,// 3a :
	0x00, 0x56, 0x36, 0x00, 0x00,// 3b ;
	0x08, 0x14, 0x22, 0x41, 0x00,// 3c <
	0x14, 0x14, 0x14, 0x14, 0x14,// 3d =
	0x00, 0x41, 0x22, 0x14, 0x08,// 3e >
	0x02, 0x01, 0x51, 0x09, 0x06,// 3f ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// 40 @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// 41 A
	0x7F, 0x49, 0x49, 0x49, 0x36,// 42 B
	0x3E, 0x41, 0x41, 0x41, 0x22,// 43 C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// 44 D
	0x7F, 0x49, 0x49, 0x49, 0x41,// 45 E
	0x7F, 0x09, 0x09, 0x09, 0x01,// 46 F
	0x3E, 0x41, 0x49, 0x49, 0x7A,// 47 G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// 48 H
	0x00, 0x41, 0x7F, 0x41, 0x00,// 49 I
	0x20, 0x40, 0x41, 0x3F, 0x01,// 4a J
	0x7F, 0x08, 0x14, 0x22, 0x41,// 4b K
	0x7F, 0x40, 0x40, 0x40, 0x40,// 4c L
	0x7F, 0x02, 0x0C, 0x02, 0x7F,// 4d M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// 4e N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// 4f O
	0x7F, 0x09, 0x09, 0x09, 0x06,// 50 P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// 51 Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// 52 R
	0x46, 0x49, 0x49, 0x49, 0x31,// 53 S
	0x01, 0x01, 0x7F, 0x01, 0x01,// 54 T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// 55 U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// 56 V
	0x3F, 0x40, 0x38, 0x40, 0x3F,// 57 W
	0x63, 0x14, 0x08, 0x14, 0x63,// 58 X
	0x07, 0x08, 0x70, 0x08, 0x07,// 59 Y
	0x61, 0x51, 0x49, 0x45, 0x43,// 5a Z
	0x00, 0x7F, 0x41, 0x41, 0x00,// 5b [
	0x02, 0x04, 0x08, 0x10, 0x20,// 5c Yen Currency Sign
	0x00, 0x41, 0x41, 0x7F, 0x00,// 5d ]
	0x04, 0x02, 0x01, 0x02, 0x04,// 5e ^
	0x40, 0x40, 0x40, 0x40, 0x40,// 5f _
	0x00, 0x01, 0x02, 0x04, 0x00,// 60 `
	0x20, 0x54, 0x54, 0x54, 0x78,// 61 a
	0x7F, 0x48, 0x44, 0x44, 0x38,// 62 b
	0x38, 0x44, 0x44, 0x44, 0x20,// 63 c
	0x38, 0x44, 0x44, 0x48, 0x7F,// 64 d
	0x38, 0x54, 0x54, 0x54, 0x18,// 65 e
	0x08, 0x7E, 0x09, 0x01, 0x02,// 66 f
	0x0C, 0x52, 0x52, 0x52, 0x3E,// 67 g
	0x7F, 0x08, 0x04, 0x04, 0x78,// 68 h
	0x00, 0x44, 0x7D, 0x40, 0x00,// 69 i
	0x20, 0x40, 0x44, 0x3D, 0x00,// 6a j
	0x7F, 0x10, 0x28, 0x44, 0x00,// 6b k
	0x00, 0x41, 0x7F, 0x40, 0x00,// 6c l
	0x7C, 0x04, 0x18, 0x04, 0x78,// 6d m
	0x7C, 0x08, 0x04, 0x04, 0x78,// 6e n
	0x38, 0x44, 0x44, 0x44, 0x38,// 6f o
	0x7C, 0x14, 0x14, 0x14, 0x08,// 70 p
	0x08, 0x14, 0x14, 0x18, 0x7C,// 71 q
	0x7C, 0x08, 0x04, 0x04, 0x08,// 72 r
	0x08, 0x54, 0x54, 0x54, 0x20,// 73 s
	0x04, 0x3F, 0x44, 0x40, 0x20,// 74 t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// 75 u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// 76 v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// 77 w
	0x44, 0x28, 0x10, 0x28, 0x44,// 78 x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// 79 y
	0x44, 0x64, 0x54, 0x4C, 0x44,// 7a z
	0x00, 0x08, 0x36, 0x41, 0x00,// 7b <
	0x00, 0x00, 0x7F, 0x00, 0x00,// 7c |
	0x00, 0x41, 0x36, 0x08, 0x00,// 7d >
	0x10, 0x08, 0x08, 0x10, 0x08,// 7e Right Arrow ->
	0x78, 0x46, 0x41, 0x46, 0x78,// 7f Left Arrow <-
	//-- 127
	//-- 192 Cyrillic
	0x7E, 0x11, 0x11, 0x11, 0x7E,// C0 ?
	0x7F, 0x49, 0x49, 0x49, 0x31,// C1 ?
	0x7F, 0x49, 0x49, 0x49, 0x36,// C2 ?
	0x7F, 0x01, 0x01, 0x01, 0x03,// C3 ?
	0x60, 0x3E, 0x21, 0x21, 0x7F,// C4 ?
	0x7F, 0x49, 0x49, 0x49, 0x41,// C5 ?
	0x77, 0x08, 0x7F, 0x08, 0x77,// C6 ?
	0x22, 0x41, 0x49, 0x49, 0x36,// C7 ?
	0x7F, 0x10, 0x08, 0x04, 0x7F,// C8 ?
	0x7F, 0x10, 0x09, 0x04, 0x7F,// C9 ?
	0x7F, 0x08, 0x14, 0x22, 0x41,// CA ?
	0x40, 0x3E, 0x01, 0x01, 0x7F,// CB ?
	0x7F, 0x02, 0x0C, 0x02, 0x7F,// CC ?
	0x7F, 0x08, 0x08, 0x08, 0x7F,// CD ?
	0x3E, 0x41, 0x41, 0x41, 0x3E,// CE ?
	0x7F, 0x01, 0x01, 0x01, 0x7F,// CF ?
	0x7F, 0x09, 0x09, 0x09, 0x06,// D0 ?
	0x3E, 0x41, 0x41, 0x41, 0x22,// D1 ?
	0x01, 0x01, 0x7F, 0x01, 0x01,// D2 ?
	0x27, 0x48, 0x48, 0x48, 0x3F,// D3 ?
	0x1E, 0x21, 0x7F, 0x21, 0x1E,// D4 ?
	0x63, 0x14, 0x08, 0x14, 0x63,// D5 ?
	0x3F, 0x20, 0x20, 0x3F, 0x60,// D6 ?
	0x07, 0x08, 0x08, 0x08, 0x7F,// D7 ?
	0x7F, 0x40, 0x7F, 0x40, 0x7F,// D8 ?
	0x3F, 0x20, 0x3F, 0x20, 0x7F,// D9 ?
	0x01, 0x7F, 0x48, 0x48, 0x30,// DA ?
	0x7F, 0x48, 0x30, 0x00, 0x7F,// DB ?
	0x00, 0x7F, 0x48, 0x48, 0x30,// DC ?
	0x22, 0x41, 0x49, 0x49, 0x3E,// DD ?
	0x7F, 0x08, 0x3E, 0x41, 0x3E,// DE ?
	0x46, 0x29, 0x19, 0x09, 0x7F,// DF ?
	0x20, 0x54, 0x54, 0x54, 0x78,// E0 ?
	0x3C, 0x4A, 0x4A, 0x4A, 0x30,// E1 ?
	0x7C, 0x54, 0x54, 0x28, 0x00,// E2 ?
	0x7C, 0x04, 0x04, 0x04, 0x04,// E3 ?
	0x60, 0x38, 0x24, 0x24, 0x7C,// E4 ?
	0x38, 0x54, 0x54, 0x54, 0x18,// E5 ?
	0x6C, 0x10, 0x7C, 0x10, 0x6C,// E6 ?
	0x00, 0x44, 0x54, 0x54, 0x28,// E7 ?
	0x7C, 0x20, 0x10, 0x08, 0x7C,// E8 ?
	0x7C, 0x21, 0x12, 0x09, 0x7C,// E9 ?
	0x7C, 0x10, 0x28, 0x44, 0x00,// EA
	0x40, 0x38, 0x04, 0x04, 0x7C,// EB
	0x7C, 0x08, 0x10, 0x08, 0x7C,// EC
	0x7C, 0x10, 0x10, 0x10, 0x7C,// ED
	0x38, 0x44, 0x44, 0x44, 0x38,// EE
	0x7C, 0x04, 0x04, 0x04, 0x7C,// EF
	0x7C, 0x14, 0x14, 0x14, 0x08,// F0
	0x38, 0x44, 0x44, 0x44, 0x00,// F1
	0x04, 0x04, 0x7C, 0x04, 0x04,// F2
	0x0C, 0x50, 0x50, 0x50, 0x3C,// F3
	0x08, 0x14, 0x7C, 0x14, 0x08,// F4
	0x44, 0x28, 0x10, 0x28, 0x44,// F5
	0x3C, 0x20, 0x20, 0x3C, 0x60,// F6
	0x0C, 0x10, 0x10, 0x10, 0x7C,// F7
	0x7C, 0x40, 0x7C, 0x40, 0x7C,// F8
	0x3C, 0x20, 0x3C, 0x20, 0x7C,// F9
	0x04, 0x7C, 0x50, 0x50, 0x20,// FA
	0x7C, 0x50, 0x20, 0x00, 0x7C,// FB
	0x00, 0x7C, 0x50, 0x50, 0x20,// FC
	0x28, 0x44, 0x54, 0x54, 0x38,// FD
	0x7C, 0x10, 0x38, 0x44, 0x38,// FE
	0x48, 0x54, 0x34, 0x14, 0x7C // FF
	// -- 255
};


//===========================================================================

static void OLED_DeInit(OLED_HandleTypeDef* OLED);

void sendCommand(uint8_t command_s)
{
	uint8_t BufferCommand[2];
	BufferCommand[0] = COMMAND;
	BufferCommand[1] = command_s;

	HAL_I2C_Master_Transmit(&hi2c1,OLED_adress,BufferCommand,2,100);
}

/*
 * @brief Function sends data to OLED unit
 *
 * @returns  Status of operation
 * 			 default : Same as HAL_I2C_Transmit
 *			 4 		: Incorrect descriptor
 *
 * @[IN]data (enum Data/Command) Descriptor
 * 			 uint8_t 			 Data to send
 */
OLED_StatusTypeDef OLED_SendData (OLED_DataType Descriptor, const uint8_t  Data){
	OLED_StatusTypeDef Result = OLED_OK;

	if(Descriptor == DATA || Descriptor == COMMAND){
		uint8_t BufferData[2];
		BufferData[0] = Descriptor;
		BufferData[1] = Data;

		Result = HAL_I2C_Master_Transmit(&hi2c1,OLED_adress,BufferData,2,100);
	}
	else{
		Result = OLED_DESCFAIL;
	}

	return Result;
}


void sendData(uint8_t data_s)
{
	uint8_t BufferData[2];
	BufferData[0] = DATA;
	BufferData[1] = data_s;

	HAL_I2C_Master_Transmit(&hi2c1,OLED_adress,BufferData,2,100);
}

void LCD_Goto(uint8_t x, uint8_t y)
{
	LCD_X = x;
	LCD_Y = y;
	sendCommand(0xB0 + y);
	sendCommand(x & 0xf);
	sendCommand(0x10 | (x >> 4));
}

/*
 * @brief Function set's all of display colors to black
 *
 * @returns  Nothing
 *
 */
void LCD_Clear(void)
{
    uint8_t temp_buffer[9] = {0,0,0,0,0,0,0,0,0};

    temp_buffer[0] = DATA;

    for (LCD_Y=0; LCD_Y <= (OLED_HEIGHT/8); LCD_Y++)
    {
        LCD_Goto(0,LCD_Y);
        for(LCD_X=0; LCD_X < (OLED_WIDTH/8); LCD_X++)
        {
            HAL_I2C_Master_Transmit(&hi2c1, OLED_adress, temp_buffer, 9,1000);
        }
    }

    LCD_X = OLED_DEFAULT_SPACE;
    LCD_Y = 0;
    LCD_Goto(0,OLED_DEFAULT_SPACE);
}

/*
 * @brief Function sets OLED contrast to desired value
 *
 * @returns  Nothing
 *
 * @[IN]data uint8_t	 Desired contrast value (0-255)
 *
 */
void Set_Contrast(uint8_t value)
{
	sendCommand(OLED_SETCONTRAST);
	sendCommand(value);
}

/*
 * @brief Function returns offset for matching ASCII
 * input with LCD_Buffer
 *
 * @returns  Offset
 *
 * @[IN]data uint8_t	 Default ASCII character
 *
 */
uint8_t Char_to_buffer_offset(uint8_t chr)
{
	if(chr >= 32 && chr <= 127)
		return 32;
	if(chr >= 192 && chr <= 255)
		return 95;
	return 0;
}

//Функция выводит 6 байт в GDDRAM OLED 7 байт - межсимвольный интервал,
//Переписать функцию чтобы она работала с кадром а не с дисплеем напрямую

void LCD_Char(uint8_t c)
{
	uint8_t x = 0;
	temp_char[0] = DATA; 	// DATA descriptor
	uint8_t offset = Char_to_buffer_offset(c);

	if(!offset) return; //If desired char is not presented in Font_6x6_RuEng exit the function

	for (x=0; x<5; x++)
	{
		temp_char[x+1] = Font_6x6_RuEng[c*5-5*offset+x];
	}
	temp_char[6] = 0;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_adress, temp_char, 7,1000);

	LCD_X += 6;
	if(LCD_X>=OLED_WIDTH-2)
	{
		if(LCD_Y > 6){
			LCD_Y = 0;
		}
		else{
			LCD_Y++;
		}
		LCD_X = OLED_DEFAULT_SPACE;
		LCD_Goto(LCD_X,LCD_Y);
	}
}


void OLED_string(char *string)
{
	while(*string != '\0')
	{
		LCD_Char(*string);
		string++;
	}
}

/*
 * @returns Nothing
 *
 * @data value binary
 * @data symbols
 *
 *
 */
void OLED_num_to_str(unsigned int value, unsigned char nDigit)
{
	switch(nDigit)
	{
		case 5: LCD_Char(value/10000+48);
		case 4: LCD_Char((value/1000)%10+48);
		case 3: LCD_Char((value/100)%10+48);
		case 2: LCD_Char((value/10)%10+48);
		case 1: LCD_Char(value%10+48);
	}
}
/*
 *
 * End function NAME_FUNCTION
 *
 */

OLED_StatusTypeDef OLED_Init(OLED_HandleTypeDef* oled)
{
	OLED_StatusTypeDef Result = OLED_OK;

	oled->DataSend = OLED_SendData;
	oled->AddressI2C = 1;

	// Turn display off
	Result = oled->DataSend(COMMAND, OLED_DISPLAYOFF);


	Result = oled->DataSend(COMMAND, OLED_SETDISPLAYCLOCKDIV);
	Result = oled->DataSend(COMMAND, OLED_DISPLAYCLOCKFREQ_MAX);


	Result = oled->DataSend(COMMAND, OLED_SETMULTIPLEX);
	Result = oled->DataSend(COMMAND, OLED_HEIGHT-1);

	Result = oled->DataSend(COMMAND, OLED_SETDISPLAYOFFSET);
	Result = oled->DataSend(COMMAND, 0x00);

	Result = oled->DataSend(COMMAND, OLED_SETSTARTLINE | OLED_STARTLINE_DEFAULT);

	// Horizontal memory mode
	Result = oled->DataSend(COMMAND, OLED_MEMORYMODE);
	Result = oled->DataSend(COMMAND, OLED_MEMORYMODE_HOR);

	Result = oled->DataSend(COMMAND, OLED_SEGREMAP | OLED_SEGREMAP_127_SEG0);


	Result = oled->DataSend(COMMAND, OLED_COMSCANDEC);

	Result = oled->DataSend(COMMAND, OLED_SETCOMPINS);
	Result = oled->DataSend(COMMAND, OLED_SETCOMPINS_COM_CONF_ALT | OLED_SETCOMPINS_COM_REMAP_NONE);


	Result = oled->DataSend(COMMAND, OLED_SETCONTRAST);
	Result = oled->DataSend(COMMAND, OLED_SETCONTRAST_MAX);


	Result = oled->DataSend(COMMAND, OLED_SETPRECHARGE);
	Result = oled->DataSend(COMMAND, OLED_SETPRECHARGE_PHASE_1 | OLED_SETPRECHARGE_PHASE_2);

	Result = oled->DataSend(COMMAND, OLED_SETVCOMDETECT);
	Result = oled->DataSend(COMMAND, OLED_SETVCOMDETECT_MID);

	Result = oled->DataSend(COMMAND, OLED_DISPLAYALLON);
	Result = oled->DataSend(COMMAND, OLED_DISPLAYALLON_RESUME);


	// Non-inverted colours of display
	Result = oled->DataSend(COMMAND, OLED_COLOURS_INV | OLED_COLOURS_INV_FALSE);

	// We use internal charge pump
	Result = oled->DataSend(COMMAND, OLED_CHARGEPUMP);
	Result = oled->DataSend(COMMAND, OLED_CHARGEPUMP_ON);

	// Turn display back on
	Result = oled->DataSend(COMMAND, OLED_DISPLAYON);

	return Result;
}

static void OLED_DeInit(OLED_HandleTypeDef* OLED){

	OLED->DataSend(COMMAND, OLED_DISPLAYOFF);

	HAL_I2C_DeInit(&hi2c1);
	MX_I2C1_Init();

}


/*
 *
 *
 * @brief Error management routine
 *
 * @returns bool status of error handeling
 *
 */

OLED_ErrorHandlerType OLED_ErrorHandler (OLED_HandleTypeDef * OLED){
	OLED_StatusTypeDef Result = OLED_OK;
	OLED_DeInit(OLED);
	if(OLED_Init(OLED) != OLED_OK && OLED->OLEDErrorSolvingTrials < OLED_MAX_TRIALS){
		OLED->OLEDErrorSolvingTrials++;
		//OLED_ErrorHandler
	}
	else{

	}

	return SUCCES;
}

