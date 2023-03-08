/*
 * ssd1306.h
 *
 *  Created on: Jan 9, 2023
 *      Author: ACA
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

/**
 * This SSD1306 LCD uses I2C for communication
 *
 * Library features functions for drawing lines, rectangles and circles.
 *
 * It also allows you to draw texts and characters using appropriate functions provided in library.
 *
 * Default pinout
 *
SSD1306    |STM32F10x    |DESCRIPTION
VCC        |3.3V         |
GND        |GND          |
SCL        |PB6          |Serial clock line
SDA        |PB7          |Serial data line
 */


#include "font.h"
#include "display.h"
#include <string>

using std::string;


/* Device address for I2C communication protocol */
#define SSD1306_DEVICE_ADDR		0x78	/* For specially my OLED device this is it's address: 0x78 */

/* SSD1306 settings */
/* SSD1306 OLED width in pixels */
#define SSD1306_WIDTH			128
/* SSD1306 OLED height in pixels */
#define SSD1306_HEIGHT			64

#ifndef SSD1306_I2C_TIMEOUT
#define SSD1306_I2C_TIMEOUT		20000	// 0x00000100 also worked fine for SSD1306_Init (function defined in ssd1306.c)
#endif

/**
 * @brief  SSD1306 color enumeration
 */
enum {
	SSD1306_COLOR_BLACK = 0x00,	/*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01	/*!< Pixel is set. Color depends on LCD */
};

enum{
	SSD1306_UNINITIALIZED = 0x00,
	SSD1306_INITIALIZED = 0x01
};

enum {
	SSD1306_OK		= DISPLAY_OK,
	SSD1306_ERROR	= DISPLAY_ERROR
};

enum {
	SSD1306_NOT_INVERTED	= 0x00,
	SSD1306_INVERTED		= 0x01
};

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))



class SSD1306{
public:
	SSD1306(void);
	~SSD1306(void);
	void FindDeviceAddress(void);
	uint8_t Init(I2C_HandleTypeDef* i2cHandler);
	uint8_t SetContrast(uint8_t value);
	void UpdateScreen(void);
	void ToggleInvert(void);
	void Fill(uint8_t color);
	void DrawPixel(uint16_t x, uint16_t y, uint8_t color);
	void GoToXY(uint16_t x, uint16_t y);
	char PutChar(char ch, font_t* font, uint8_t color);
	uint8_t PutSymbol(const uint8_t& symbol, font_t* font, uint8_t color);
	void PutString(string str, font_t* font, uint8_t color);
	void PutComb(const uint8_t* comb, font_t* font, uint8_t color);
	void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
	void DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
	void DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
	void DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color);
	void DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color);
	void DrawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void I2CWrite(uint8_t address, uint8_t reg, uint8_t data);
	void I2CWriteMulti(uint8_t* data, uint16_t count);
	void TurnON(void);
	void TurnOFF(void);
	uint8_t SendCommand(uint8_t command);
	uint8_t SendData(uint8_t* data, uint16_t count);
	uint8_t SendRowData(uint8_t* data);



private:
	I2C_HandleTypeDef* i2ch_;
	uint8_t devAddr_;
	uint16_t x_;
	uint16_t y_;
	uint8_t inverted_;
	uint8_t init_;
	uint8_t buff_[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
};


#endif /* INC_SSD1306_H_ */
