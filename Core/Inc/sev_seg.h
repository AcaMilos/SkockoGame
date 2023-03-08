/*
 * sev_seg.h
 *
 *  Created on: 10 Jan 2023
 *      Author: ACA
 */

#ifndef INC_SEV_SEG_H_
#define INC_SEV_SEG_H_

#include "main.h"
#include "display.h"

#define SYMBOLS_NUMBER 18

typedef enum{
	SEGMENT_OFF = GPIO_PIN_SET,
	SEGMENT_ON	= GPIO_PIN_RESET
} segment_t;



class SEV_SEG : Display{
public:
	SEV_SEG(const SEV_SEG& obj) = delete;
	~SEV_SEG(void);
	static SEV_SEG* GetInstance(void);

	uint8_t Init(TIM_HandleTypeDef* TimerForMultiplexing, uint8_t data1, uint8_t data2);
	void Print(uint8_t data);
	uint8_t Clear(void) override;
	uint8_t TurnON(void) override;
	uint8_t TurnOFF(void) override;
	void Increment(void);
	uint8_t UpdateDisplay(void);
	uint8_t SwitchDisplay(void);

	enum { DISPLAY_OFF = GPIO_PIN_SET, DISPLAY_ON = GPIO_PIN_RESET };
	enum { LETTER_A	= 0x0A, LETTER_B = 0x0B, LETTER_C = 0x0C, LETTER_D = 0x0D, LETTER_E	= 0x0E, LETTER_F = 0x0F, LETTER_NONE = 0x10, LETTER_L = 0x11 };
	enum { DISPLAY_1 = 0x01, DISPLAY_2 = 0x02 };
	enum { SEV_SEG_UNINITIALIZED = 0x00, SEV_SEG_INITIALIZED = 0x01 };
	enum { SEV_SEG_OK = DISPLAY_OK, SEV_SEG_ERROR = DISPLAY_ERROR };
	enum { SEV_SEG_OFF = 0x00, SEV_SEG_ONE_ACTIVE = 0x01, SEV_SEG_BOOTH_ACTIVE = 0x02 };

private:
	SEV_SEG(void);
	SEV_SEG(TIM_HandleTypeDef* htimX, uint8_t data1, uint8_t data2);
	static SEV_SEG* instancePointerSEV_SEG;
	TIM_HandleTypeDef* muxTIM_;
	uint8_t data1_;
	uint8_t data2_;
	uint8_t currDisp_;
	uint8_t init_;
	uint8_t status_;
};


const segment_t sa[SYMBOLS_NUMBER] = {
		SEGMENT_ON,		// 0
		SEGMENT_OFF,	// 1
		SEGMENT_ON,		// 2
		SEGMENT_ON,		// 3
		SEGMENT_OFF,	// 4
		SEGMENT_ON,		// 5
		SEGMENT_ON,		// 6
		SEGMENT_ON,		// 7
		SEGMENT_ON,		// 8
		SEGMENT_ON,		// 9
		SEGMENT_ON,		// A
		SEGMENT_OFF,	// B
		SEGMENT_ON,		// C
		SEGMENT_OFF,	// D
		SEGMENT_ON,		// E
		SEGMENT_ON,		// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_OFF		// L
};

const segment_t sb[SYMBOLS_NUMBER] = {
		SEGMENT_ON,		// 0
		SEGMENT_ON,		// 1
		SEGMENT_ON,		// 2
		SEGMENT_ON,		// 3
		SEGMENT_ON,		// 4
		SEGMENT_OFF,	// 5
		SEGMENT_OFF,	// 6
		SEGMENT_ON,		// 7
		SEGMENT_ON,		// 8
		SEGMENT_ON,		// 9
		SEGMENT_ON,		// A
		SEGMENT_OFF,	// B
		SEGMENT_OFF,	// C
		SEGMENT_ON,		// D
		SEGMENT_OFF,	// E
		SEGMENT_OFF,	// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_OFF		// L
};

const segment_t sc[SYMBOLS_NUMBER] = {
		SEGMENT_ON,		// 0
		SEGMENT_ON,		// 1
		SEGMENT_OFF,	// 2
		SEGMENT_ON,		// 3
		SEGMENT_ON,		// 4
		SEGMENT_ON,		// 5
		SEGMENT_ON,		// 6
		SEGMENT_ON,		// 7
		SEGMENT_ON,		// 8
		SEGMENT_ON,		// 9
		SEGMENT_ON,		// A
		SEGMENT_ON,		// B
		SEGMENT_OFF,	// C
		SEGMENT_ON,		// D
		SEGMENT_OFF,	// E
		SEGMENT_OFF,	// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_OFF		// L
};

const segment_t sd[SYMBOLS_NUMBER] = {
		SEGMENT_ON,		// 0
		SEGMENT_OFF,	// 1
		SEGMENT_ON,		// 2
		SEGMENT_ON,		// 3
		SEGMENT_OFF,	// 4
		SEGMENT_ON,		// 5
		SEGMENT_ON,		// 6
		SEGMENT_OFF,	// 7
		SEGMENT_ON,		// 8
		SEGMENT_ON,		// 9
		SEGMENT_OFF,	// A
		SEGMENT_ON,		// B
		SEGMENT_ON,		// C
		SEGMENT_ON,		// D
		SEGMENT_ON,		// E
		SEGMENT_OFF,	// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_ON		// L
};

const segment_t se[SYMBOLS_NUMBER] = {
		SEGMENT_ON,		// 0
		SEGMENT_OFF,	// 1
		SEGMENT_ON,		// 2
		SEGMENT_OFF,	// 3
		SEGMENT_OFF,	// 4
		SEGMENT_OFF,	// 5
		SEGMENT_ON,		// 6
		SEGMENT_OFF,	// 7
		SEGMENT_ON,		// 8
		SEGMENT_OFF,	// 9
		SEGMENT_ON,		// A
		SEGMENT_ON,		// B
		SEGMENT_ON,		// C
		SEGMENT_ON,		// D
		SEGMENT_ON,		// E
		SEGMENT_ON,		// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_ON		// L
};

const segment_t sf[SYMBOLS_NUMBER] = {
		SEGMENT_ON,		// 0
		SEGMENT_OFF,	// 1
		SEGMENT_OFF,	// 2
		SEGMENT_OFF,	// 3
		SEGMENT_ON,		// 4
		SEGMENT_ON,		// 5
		SEGMENT_ON,		// 6
		SEGMENT_OFF,	// 7
		SEGMENT_ON,		// 8
		SEGMENT_ON,		// 9
		SEGMENT_ON,		// A
		SEGMENT_ON,		// B
		SEGMENT_ON,		// C
		SEGMENT_OFF,	// D
		SEGMENT_ON,		// E
		SEGMENT_ON,		// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_ON		// L
};

const segment_t sg[SYMBOLS_NUMBER] = {
		SEGMENT_OFF,	// 0
		SEGMENT_OFF,	// 1
		SEGMENT_ON,		// 2
		SEGMENT_ON,		// 3
		SEGMENT_ON,		// 4
		SEGMENT_ON,		// 5
		SEGMENT_ON,		// 6
		SEGMENT_OFF,	// 7
		SEGMENT_ON,		// 8
		SEGMENT_ON,		// 9
		SEGMENT_ON,		// A
		SEGMENT_ON,		// B
		SEGMENT_OFF,	// C
		SEGMENT_ON,		// D
		SEGMENT_ON,		// E
		SEGMENT_ON,		// F
		SEGMENT_OFF,	// display_OFF
		SEGMENT_OFF		// L
};

#endif /* INC_SEV_SEG_H_ */
