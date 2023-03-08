/*
 * oled.cpp
 *
 *  Created on: Jan 9, 2023
 *      Author: ACA
 */



#include "oled.h"


/**
  * @brief
  * @note
  * @param
  * @retval
  */
OLED::OLED(void) : init_(this->UNINITIALIZED){
	this->i2ch_ = new I2C_HandleTypeDef;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
OLED::~OLED(void){
	delete i2ch_;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
OLED* OLED::GetInstance(void){
	if(instancePointerOLED == nullptr){
		instancePointerOLED = new OLED;
		return instancePointerOLED;
	}
	else{
		return instancePointerOLED;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t OLED::Init(I2C_HandleTypeDef* i2cHandler){
	this->i2ch_ = i2cHandler;
	if(this->mySSD1306_.Init(this->i2ch_) != OLED_OK){
		return OLED_ERROR;
	}
	else{
		this->OLED::init_ = this->INITIALIZED;
		return OLED_OK;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t OLED::Clear(void){
	this->mySSD1306_.Fill(SSD1306_COLOR_BLACK);
	return OLED_OK;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void OLED::PrintString(const string& message, const disp_pos_t& pos){
	this->mySSD1306_.GoToXY(pos.x_, pos.y_);
	this->mySSD1306_.PutString(message, &font_7x10, this->PIXEL_YELLOW);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void OLED::PrintSkockoComb(const uint8_t* comb, const uint8_t& currPos, const uint8_t& dispRow, const uint8_t& underline){
	this->mySSD1306_.GoToXY(this->FIRST_COL, dispRow);
	this->mySSD1306_.PutComb(comb, &skocko_7x10, this->PIXEL_YELLOW);
	if(underline == this->WITH_UNDERLINE){
		uint8_t symbolUnderline[4];
		switch(currPos){
		case 0:
			symbolUnderline[0] = 7;
			symbolUnderline[1] = 0;
			symbolUnderline[2] = 0;
			symbolUnderline[3] = 0;
			break;
		case 1:
			symbolUnderline[0] = 0;
			symbolUnderline[1] = 7;
			symbolUnderline[2] = 0;
			symbolUnderline[3] = 0;
			break;
		case 2:
			symbolUnderline[0] = 0;
			symbolUnderline[1] = 0;
			symbolUnderline[2] = 7;
			symbolUnderline[3] = 0;
			break;
		case 3:
			symbolUnderline[0] = 0;
			symbolUnderline[1] = 0;
			symbolUnderline[2] = 0;
			symbolUnderline[3] = 7;
			break;
		default:
			break;
		}
		this->mySSD1306_.GoToXY(this->FIRST_COL, this->THIRD_ROW);
		this->mySSD1306_.PutComb(symbolUnderline, &skocko_7x10, this->PIXEL_YELLOW);
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
//void OLED::PrintPicture(const uint16_t* picture_pixels){
//	this->mySSD1306_.SendRowData(picture_pixels);
//}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void OLED::UpdateScreen(void){
	this->mySSD1306_.UpdateScreen();
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void OLED::SetBrightness(uint8_t value){
	this->mySSD1306_.SetContrast(value);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t OLED::TurnOFF(void){
	this->mySSD1306_.TurnOFF();
	return OLED_OK;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t OLED::TurnON(void){
	this->mySSD1306_.TurnON();
	return OLED_OK;
}
