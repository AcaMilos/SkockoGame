/*
 * display.h
 *
 *  Created on: Jan 9, 2023
 *      Author: ACA
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


#include "main.h"

enum{
	DISPLAY_OK		= 0x01,
	DISPLAY_ERROR	= 0x02
};


class Display{
public:
//	virtual Display() = 0;
//	virtual ~Display() = 0;
	virtual uint8_t Clear(void) = 0;
	virtual uint8_t TurnON(void) = 0;
	virtual uint8_t TurnOFF(void) = 0;
protected:
};




#endif /* INC_DISPLAY_H_ */
