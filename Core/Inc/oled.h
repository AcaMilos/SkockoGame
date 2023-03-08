/*
 * oled.h
 *
 *  Created on: Jan 9, 2023
 *      Author: ACA
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "display.h"
#include "ssd1306.h"
#include <string>

using std::string;


enum {
	OLED_OK		= DISPLAY_OK,
	OLED_ERROR	= DISPLAY_ERROR
};

typedef struct disp_pos{
	 uint8_t x_;
	 uint8_t y_;
} disp_pos_t;


class OLED : public Display{
public:
	OLED(const OLED& obj) = delete;
	~OLED(void);
	static OLED* GetInstance(void);

	uint8_t Init(I2C_HandleTypeDef* i2cHandler);
	uint8_t Clear(void) override;
	void PrintString(const string& message, const disp_pos_t& pos);
	void PrintSkockoComb(const uint8_t* comb, const uint8_t& currPos, const uint8_t& dispRow, const uint8_t& underline);
	void UpdateScreen(void);
	void SetBrightness(uint8_t value);
//	uint8_t Print(const string header, const string first_line, const string second_line, const string third_line);
//	uint8_t Print(const string header, const string first_line, const uint8_t* comb, const uint8_t& currPos);
//	void PrintText(const string* text);
//	void PrintString(const uint8_t& xPos, const uint8_t& yPos, const string message);
//	void PrintSymbol(const string message);
//	void PrintPicture(const uint16_t* picture_pixels);
	//void PrintError(string message);
	uint8_t TurnON(void) override;
	uint8_t TurnOFF(void) override;

	enum { PIXEL_BLUE = 0x00, PIXEL_YELLOW = 0x01 };
	enum { FIRST_COL = 5, SECOND_COL = 36, THIRD_COL = 68, FOURTH_COL = 100 };
	enum { FIRST_ROW = 5, SECOND_ROW = 20, THIRD_ROW = 35, FOURTH_ROW = 50 };
	enum { UNINITIALIZED = 0x00, INITIALIZED = 0x01 };
	enum { NO_UNDERLINE = 0x00, WITH_UNDERLINE = 0x01};
	//friend class Skocko; **** I want to friend these two classes somehow to be able to specialize Print function of OLED for Skocko game.
	//Becouse now its not natural, I mean this now my OLED class is adopted directly to the Skocko game. I wanted to give Skocko class functionalities
	//from OLED class, so I could in Skocko class specify special functions like printing current combination, or printing winning picture, or
	//printing winning message, and etc. ...

private:
	OLED(void);
	static OLED* instancePointerOLED;
	I2C_HandleTypeDef* i2ch_;
	uint8_t init_;
	SSD1306 mySSD1306_;
};



#endif /* INC_OLED_H_ */
