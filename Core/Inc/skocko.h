/*
 * skocko.h
 *
 *  Created on: Jan 14, 2023
 *      Author: ACA
 */

#ifndef INC_SKOCKO_H_
#define INC_SKOCKO_H_

#include "main.h"
#include <string>
#include "oled.h"
#include "sev_seg.h"

using std::string;


class Skocko{
public:
	Skocko(const Skocko& obj) = delete;
	~Skocko(void);
	static Skocko* GetInstance(I2C_HandleTypeDef* i2cForDisplayHandler);

	void Init(TIM_HandleTypeDef* htimOLED, TIM_HandleTypeDef* htimCombGen, UART_HandleTypeDef* huart);
	void PrintIntro(void);
	void SetupUser(void);
	void SetupBrightness(void);
	void PrintGameInfo(void);
	void StartSkocko(void);
	void NewComb(void);
	void ResetGame(void);
	uint8_t GetState(void);
	/* Setter methods */
	void SetWinFactor(const uint16_t& winFactor);
	void SetSymbol(const uint8_t& position, const uint8_t& symbol);
	void SetBrightness(const uint32_t& value);
	/* Getter methods */
	uint8_t GetCurrentPosition(void);
	uint8_t GetSymbol(const uint8_t& position);

	void ConfirmBrightness(void);
	void IncrementSymbol(const uint8_t& position);
	void IncrementPosition(void);
	void DecrementPosition(void);
	void RefreshOLED(void);
	void ConfirmComb(void);

	/* In progress... */
	//void PrintHelp(void);

	void operator+(const uint8_t& position);

	enum { NONE = 0x00, SKOCKO = 0x01, ZVEZDA = 0x02, PIK = 0x03, HERC = 0x04, TREF = 0x05, KARO = 0x06 };
	enum { POSITION_1 = 0x00, POSITION_2 = 0x01, POSITION_3 = 0x02, POSITION_4 = 0x03 };
	enum { INPUT_VALID = 0x01, INPUT_INVALID = 0x02};
	enum { ST_BEGIN, ST_WAITING_PLAYER_NAME, ST_WAITING_BRIGHTNESS, ST_WAITING_TO_START, ST_RECEIVING_COMB, ST_WAITING_NEXT_COMB, ST_GAME_END};
	static const uint8_t COMB_SYMBOLS_CNT = 4;
	static const uint8_t MAX_TRIES = 6;
	static const uint8_t SYMBOLS_CNT = 6;
	static const uint8_t PLAYER_NAME_MAX_LENGTH = 20;

private:
	Skocko(void);
	Skocko(I2C_HandleTypeDef* i2cForDisplayHandler);
	static Skocko* instancePointerSkocko;
	UART_HandleTypeDef* huart_;
	TIM_HandleTypeDef* timerOLED_;
	TIM_HandleTypeDef* timerWinCombGen_;
	uint8_t state_;
	uint8_t winComb_[COMB_SYMBOLS_CNT];
	uint16_t winFactor_;
	uint8_t playerName_[PLAYER_NAME_MAX_LENGTH];
	uint8_t currPos_;
	uint8_t currComb_[COMB_SYMBOLS_CNT];
	uint8_t currTry_;
	uint16_t mask_[COMB_SYMBOLS_CNT];
	OLED* OLED_;
	SEV_SEG* SEV_SEG_;

	struct result{
		uint8_t correct_;
		uint8_t half_correct_;
	};

	struct symbCnt{
		uint8_t skockoCnt_;
		uint8_t zvezdaCnt_;
		uint8_t pikCnt_;
		uint8_t hercCnt_;
		uint8_t trefCnt_;
		uint8_t karoCnt_;
	};

	struct combCnt{
		symbCnt winComb;
		symbCnt userComb;
	};

	void WinCombGen(void);
	uint8_t* GetComb(void);
	uint8_t InputCheck(void);
	result CalculateResult(void);
	combCnt CountSymbols(void);
	void PrintResult(const result& res);
	void PrintWinComb(const uint8_t& dispRow);
	void PrintUART(const string& message);
	void PrintUART(const uint8_t& digit);
	void EndGame(const result& res);
	void PrintCombToUART(const uint8_t* comb);
	void PrintPlayerNameToUART(void);
	void ReceivePlayerNameFromUART(void);

	/* In progress... */
	//void TurnBuzzerON(void);

};



#endif /* INC_SKOCKO_H_ */
