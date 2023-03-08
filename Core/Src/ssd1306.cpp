/*
 * ssd1306.cpp
 *
 *  Created on: Jan 9, 2023
 *      Author: ACA
 */


#include "ssd1306.h"


/**
  * @brief
  * @note
  * @param
  * @retval
  */
SSD1306::SSD1306(void){
	i2ch_ = new I2C_HandleTypeDef;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
SSD1306::~SSD1306(void){
	delete i2ch_;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::FindDeviceAddress(void){

	uint8_t devAddr = 0;

	while(devAddr < 128){
		if (HAL_I2C_IsDeviceReady(this->i2ch_, devAddr, 1, 0x00000100) == HAL_OK){
			break;
		}
		devAddr++;
	}

	this->devAddr_ = devAddr;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t SSD1306::Init(I2C_HandleTypeDef* i2cHandler){

	/* Set default values */
	this->x_ = 0;
	this->y_ = 0;
	this->init_ = SSD1306_UNINITIALIZED;
	this->inverted_ = SSD1306_NOT_INVERTED;

	this->i2ch_ = i2cHandler;
	this->FindDeviceAddress();

	/* A little delay */
	uint32_t start_delay_iterator = 2500;
	while(start_delay_iterator > 0){
		start_delay_iterator--;
	}

	/*
	 *					****	Init LCD	****
	 *	For more info consult: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
	 *	pages 28 to 32 contain tables with commands, and bellow is additional description of commands
	 *
	 *										// DEFAULT VALUES (after reset) is the first row and bellow row is command description
	 * */									// -	, single command
	if(this->SendCommand(0xAE) != HAL_OK){	// Display OFF (sleep mode)
		return SSD1306_ERROR;
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0x20) != HAL_OK){	// Set Memory Addressing Mode (next sent command will adjust which one!)
		return SSD1306_ERROR;
	}										// 0x10	, double Byte command : second Byte
	if(this->SendCommand(0x10) != HAL_OK){	// 00 = Horizontal Addressing Mode, 01 = Vertical Addressing Mode
		return SSD1306_ERROR;														// 10,Page Addressing Mode (RESET), 11,Invalid
	}										// -	, single command
	if(this->SendCommand(0xB0) != HAL_OK){	// Set Page Start Address for Page Addressing Mode,0-7
		return SSD1306_ERROR;														// 0xB0 = PAGE0 , ... , 0xB7 = PAGE7
	}										// 0xC0	, single command
	if(this->SendCommand(0xC8) != HAL_OK){	// Set COM Output Scan Direction: 0xC8 = remapped mode, 0xC0 = normal mode
		return SSD1306_ERROR;														// For example, if this command is sent during normal display then the
																					// graphic display will be vertically flipped immediately
	}										// -	, single command
	if(this->SendCommand(0x00) != HAL_OK){	// Sets lower nibble of column address (this command is only for page addressing mode)
		return SSD1306_ERROR;														// in this case 0x00 will give 0000 nibble
	}										// -	, single command
	if(this->SendCommand(0x10) != HAL_OK){	// Set higher nibble of column address
		return SSD1306_ERROR;														// in this case 0x00 will give 0000 nibble
																					// which will result to COLUMN_0
	}										// 0x40	, single command
	if(this->SendCommand(0x40) != HAL_OK){	// Set display start line - Set display RAM display start line register from
		return SSD1306_ERROR;														// 0-63 using lower 6 bits of this command which are in this case = 00 0000b
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0x81) != HAL_OK){	// Set contrast control with next command
		return SSD1306_ERROR;
	}										// 0x7F	, double Byte command : second Byte
	if(this->SendCommand(0x7F) != HAL_OK){	// Contrast value (RESET = 0x7F)
		return SSD1306_ERROR;														// as much this value is higher, contrast will be higher
	}										// 0xA0	, single command
	if(this->SendCommand(0xA1) != HAL_OK){	// Set segment Re-map
		return SSD1306_ERROR;														// 0xA1 = column address 127 will be mapped to SEG0
	}										// 0xA6	, single command
	if(this->SendCommand(0xA6) != HAL_OK){	// Set normal/inverse display
		return SSD1306_ERROR;														// 0xA6 = Normal, 0xA7 = Inverse
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0xA8) != HAL_OK){	// Set multiplex ratio (from 16 to 64, 0 to 15 are invalid)
		return SSD1306_ERROR;
	}										// 0x3F	, double Byte command : second Byte
	if(this->SendCommand(0x3F) != HAL_OK){	// 0x3F = 63 decimally => MUX ratio =  64
		return SSD1306_ERROR;
	}										// 0xA4 , single command
	if(this->SendCommand(0xA4) != HAL_OK){	//0xA4 = Output follows RAM content, 0xA5 = Output ignores RAM content (ENTIRE DISPLAY ON)
		return SSD1306_ERROR;
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0xD3) != HAL_OK){	// Set display offset: set vertical shift by COM from 0d~63d
		return SSD1306_ERROR;
	}										// 0x00	, double Byte command : second Byte
	if(this->SendCommand(0x00) != HAL_OK){	// 0x00 = Not offset
		return SSD1306_ERROR;
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0xD5) != HAL_OK){	// Set display clock divide ratio/oscillator frequency
		return SSD1306_ERROR;
	}										// 0x80	, double Byte command : second Byte
	if(this->SendCommand(0xF0) != HAL_OK){	// divide ratio = 1, oscillator frequency = MAX
		return SSD1306_ERROR;
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0xD9) != HAL_OK){	// Set pre-charge period
		return SSD1306_ERROR;
	}										// 0x22	, double Byte command : second Byte
	if(this->SendCommand(0x22) != HAL_OK){	// phase1 and phase2 adjustment = by default
		return SSD1306_ERROR;
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0xDA) != HAL_OK){	// Set COM pins hardware configuration
		return SSD1306_ERROR;
	}										// 0x12	, double Byte command : second Byte
	if(this->SendCommand(0x12) != HAL_OK){	// => alternative COM pin configuration, disable COM left/right re-map
		return SSD1306_ERROR;
	}										// -	, double Byte command : first Byte
	if(this->SendCommand(0xDB) != HAL_OK){	// Set Vcomh deselect level
		return SSD1306_ERROR;
	}										// 0x20	, double Byte command : second Byte
	if(this->SendCommand(0x20) != HAL_OK){	// =>Vcomh deselect level = 0.77xVcc
		return SSD1306_ERROR;
	}										//
	if(this->SendCommand(0x8D) != HAL_OK){	//--set DC-DC enable
		return SSD1306_ERROR;
	}										//
	if(this->SendCommand(0x14) != HAL_OK){ //
		return SSD1306_ERROR;
	}										// -	, single command
	if(this->SendCommand(0xAF) != HAL_OK){	// Turn on SSD1306 panel
		return SSD1306_ERROR;
	}

	/* Clear screen */
	this->Fill(SSD1306_COLOR_WHITE);
	/* Update screen */
	this->UpdateScreen();


	/* After initialization if all commands have passed we can assume that SSD1306 OLED is initalized. */
	this->init_ = SSD1306_INITIALIZED;

	return SSD1306_OK;
}


uint8_t SSD1306::SetContrast(uint8_t value){
	// -	, double Byte command : first Byte
	if(this->SendCommand(0x81) != HAL_OK){	// Set contrast control with next command
		return SSD1306_ERROR;
	}										// 0x7F	, double Byte command : second Byte
	if(this->SendCommand(value) != HAL_OK){	// Contrast value (RESET = 0x7F)
		return SSD1306_ERROR;				// as much this value is higher, contrast will be higher
	}
	return SSD1306_OK;
}



/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::UpdateScreen(void) {
	uint8_t m;

	for (m = 0; m < 8; m++) {
		/* This Code is following datasheet on page 34 */
		if(this->SendCommand(0xB0 + m) != HAL_OK){
			return;
		}
		if(this->SendCommand(0x00) != HAL_OK){
			return;
		}
		if(this->SendCommand(0x10) != HAL_OK){
			return;
		}

		/* Write multi data */
		this->SendData(&buff_[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::ToggleInvert(void) {
	uint16_t i;

	/* Toggle invert */
	this->inverted_ = !this->inverted_;

	/* Do memory toggle */
	for (i = 0; i < sizeof(this->buff_); i++) {
		this->buff_[i] = ~(this->buff_[i]);
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::Fill(uint8_t color) {
	/* Set memory */
	if (color == SSD1306_COLOR_BLACK){
		for(uint16_t i = 0; i < sizeof(this->buff_); i++){
			this->buff_[i] = 0x00;
		}
	}
	else{
		for(uint16_t i = 0; i < sizeof(this->buff_); i++){
			this->buff_[i] = 0xFF;
		}
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawPixel(uint16_t x, uint16_t y, uint8_t color) {
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		/* Error */
		return;
	}

	/* Check if pixels are inverted */
	if (this->inverted_) {
		color = (uint8_t)!color;
	}

	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		this->buff_[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	}
	else {
		this->buff_[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::GoToXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	this->x_ = x;
	this->y_ = y;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
char SSD1306::PutChar(char ch, font_t* font, uint8_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
	if (SSD1306_WIDTH <= (this->x_ + font->font_width) || SSD1306_HEIGHT <= (this->y_ + font->font_height)) {
		/* Error */
		return 0;
	}

	/* Go through font */
	for (i = 0; i < font->font_height; i++) {
		b = font->data[(ch - 32) * font->font_height + i];
		for (j = 0; j < font->font_width; j++) {
			if ((b << j) & 0x8000) {
				this->DrawPixel(this->x_ + j, (this->y_ + i), (uint8_t) color);
			}
			else {
				this->DrawPixel(this->x_ + j, (this->y_ + i), (uint8_t)!color);
			}
		}
	}

	/* Increase pointer */
	this->x_ += font->font_width;

	/* Return character written */
	return ch;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t SSD1306::PutSymbol(const uint8_t& symbol, font_t* font, uint8_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
	if (SSD1306_WIDTH <= (this->x_ + font->font_width) || SSD1306_HEIGHT <= (this->y_ + font->font_height)) {
		/* Error */
		return 0;
	}

	/* Go through font */
	for (i = 0; i < font->font_height; i++) {
		b = font->data[symbol * font->font_height + i];
		for (j = 0; j < font->font_width; j++) {
			if ((b << j) & 0x8000) {
				this->DrawPixel(this->x_ + j, (this->y_ + i), (uint8_t) color);
			}
			else {
				this->DrawPixel(this->x_ + j, (this->y_ + i), (uint8_t)!color);
			}
		}
	}

	/* Increase pointer */
	this->x_ += font->font_width;

	/* Return character written */
	return symbol;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::PutString(string str, font_t* font, uint8_t pix_color){
	/* Write characters */
	uint8_t i = 0;
	char ch = str[0];
	while (ch != '\0') {
		/* Write character by character */
		if (this->PutChar(ch, font, pix_color) != ch) {
			/* Return error */
			return;
		}

		/* Increase string pointer */
		i++;
		ch = str[i];
	}

	/* Everything OK, zero should be returned */
	return;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::PutComb(const uint8_t* comb, font_t* font, uint8_t pix_color){
	/* Write characters */
	uint8_t i = 0;
	uint8_t symbol = comb[0];
	while (i < 4) {
		/* Write character by character */
		if (this->PutSymbol(symbol, font, pix_color) != symbol) {
			/* Return error */
			return;
		}

		/* Increase string pointer */
		i++;
		symbol = comb[i];
	}

	/* Everything OK, zero should be returned */
	return;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color){
	int16_t dx, dy, sx, sy, err, e2, i, tmp;

	/* Check for overflow */
	if (x0 >= SSD1306_WIDTH) {
		x0 = SSD1306_WIDTH - 1;
	}
	if (x1 >= SSD1306_WIDTH) {
		x1 = SSD1306_WIDTH - 1;
	}
	if (y0 >= SSD1306_HEIGHT) {
		y0 = SSD1306_HEIGHT - 1;
	}
	if (y1 >= SSD1306_HEIGHT) {
		y1 = SSD1306_HEIGHT - 1;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			this->DrawPixel(x0, i, color);
		}

		/* Return from function */
		return;
	}

	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			this->DrawPixel(i, y0, color);
		}

		/* Return from function */
		return;
	}

	while (1) {
		this->DrawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color){
	/* Check input parameters */
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}

	/* Draw 4 lines */
	this->DrawLine(x, y, x + w, y, color);         /* Top line */
	this->DrawLine(x, y + h, x + w, y + h, color); /* Bottom line */
	this->DrawLine(x, y, x, y + h, color);         /* Left line */
	this->DrawLine(x + w, y, x + w, y + h, color); /* Right line */
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t c){
	uint8_t i;

	/* Check input parameters */
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		this->DrawLine(x, y + i, x + w, y + i, c);
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color){
	/* Draw lines */
	this->DrawLine(x1, y1, x2, y2, color);
	this->DrawLine(x2, y2, x3, y3, color);
	this->DrawLine(x3, y3, x1, y1, color);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color){
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		this->DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color){
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	this->DrawPixel(x0, y0 + r, color);
	this->DrawPixel(x0, y0 - r, color);
	this->DrawPixel(x0 + r, y0, color);
	this->DrawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		this->DrawPixel(x0 + x, y0 + y, color);
		this->DrawPixel(x0 - x, y0 + y, color);
		this->DrawPixel(x0 + x, y0 - y, color);
		this->DrawPixel(x0 - x, y0 - y, color);

		this->DrawPixel(x0 + y, y0 + x, color);
		this->DrawPixel(x0 - y, y0 + x, color);
		this->DrawPixel(x0 + y, y0 - x, color);
		this->DrawPixel(x0 - y, y0 - x, color);
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color){
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	this->DrawPixel(x0, y0 + r, color);
	this->DrawPixel(x0, y0 - r, color);
	this->DrawPixel(x0 + r, y0, color);
	this->DrawPixel(x0 - r, y0, color);
	this->DrawPixel(x0, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		this->DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		this->DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

		this->DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
		this->DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::TurnON(void) {
	this->SendCommand(0x8D);
	this->SendCommand(0x14);
	this->SendCommand(0xAF);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void SSD1306::TurnOFF(void) {
	this->SendCommand(0x8D);
	this->SendCommand(0x10);
	this->SendCommand(0xAE);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t SSD1306::SendCommand(uint8_t command){
	uint8_t command_buff[2];
	command_buff[0] = 0x00;
	command_buff[1] = command;
	uint8_t command_status = 0x03;	/* If nothing responds at HAL_I2C_Master_Transmit, but it will always I assume, this will be return value. */

	command_status = HAL_I2C_Master_Transmit(this->i2ch_, this->devAddr_, command_buff, 0x0002, SSD1306_I2C_TIMEOUT);
	return command_status;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t SSD1306::SendData(uint8_t* data, uint16_t count){

	uint8_t command_status = SSD1306_ERROR;

	if(count == SSD1306_WIDTH){
		command_status = this->SendRowData(data);
		return command_status;
	}
	else{
		return SSD1306_ERROR;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t SSD1306::SendRowData(uint8_t* data){

	uint8_t data_buff[SSD1306_WIDTH+1];
	data_buff[0] = 0x40;
	for(int i = 0; i < SSD1306_WIDTH; i++){
		data_buff[i + 1] = *(data + i);
	}

	uint8_t command_status = 0x03;	/* If nothing responds at HAL_I2C_Master_Transmit, but it will always I assume, this will be return value. */

	//data_buff[0] = 0xC0;
	//command_status = HAL_I2C_Master_Transmit_DMA(this->i2ch_, this->devAddr_, data_buff, SSD1306_WIDTH + 1);
	command_status = HAL_I2C_Master_Transmit(this->i2ch_, this->devAddr_, data_buff, SSD1306_WIDTH + 1, SSD1306_I2C_TIMEOUT);

	//for(int j = 0; j < 1000; j++);

	return command_status;
}
