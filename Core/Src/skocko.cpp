/*
 * skocko.cpp
 *
 *  Created on: Jan 14, 2023
 *      Author: ACA
 */


#include "skocko.h"

#include "i2c.h"



OLED* OLED::instancePointerOLED = nullptr;

SEV_SEG* SEV_SEG::instancePointerSEV_SEG = nullptr;


/**
  * @brief
  * @note
  * @param
  * @retval
  */
Skocko::Skocko(I2C_HandleTypeDef* i2cForDisplayHandler) : winComb_{0}, winFactor_(0x5734), currPos_{0}, currComb_{0}, currTry_(0), mask_{0x000F, 0x00F0, 0x0F00, 0xF000} {

	this->state_ = this->ST_BEGIN;
	this->OLED_ = this->OLED_->GetInstance();
	this->OLED_->Init(i2cForDisplayHandler);
	this->SEV_SEG_ = this->SEV_SEG_->GetInstance();
	this->SEV_SEG_->Print(1);
	this->huart_ = new UART_HandleTypeDef;
	memset(this->playerName_, 0, sizeof(this->playerName_));
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
Skocko::~Skocko(void){
	delete this->huart_;
	delete this->instancePointerSkocko;
	delete this->timerOLED_;
	delete this->timerWinCombGen_;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
Skocko* Skocko::GetInstance(I2C_HandleTypeDef* i2cForDisplayHandler){
	if(instancePointerSkocko == nullptr){
		instancePointerSkocko = new Skocko(i2cForDisplayHandler);
		return instancePointerSkocko;
	}
	else{
		return instancePointerSkocko;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::Init(TIM_HandleTypeDef* htimOLED, TIM_HandleTypeDef* htimCombGen, UART_HandleTypeDef* huart){
	this->huart_ = huart;
	this->timerOLED_ = htimOLED;
	this->timerWinCombGen_ = htimCombGen;
	this->state_ = this->ST_WAITING_PLAYER_NAME;
	/* Start timer for WinningCombinationGeneration */
	HAL_TIM_Base_Start(this->timerWinCombGen_);
	/* Startup window */
	this->PrintIntro();
	this->SetupUser();
	this->SetupBrightness();
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintIntro(void){
	/* OLED display */
	this->OLED_->Clear();
	disp_pos_t info1_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::FIRST_ROW};
	this->OLED_->PrintString("Dobro dosli u", info1_pos);
	disp_pos_t info2_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::SECOND_ROW};
	this->OLED_->PrintString("Slagalicu!", info2_pos);
	disp_pos_t info3_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::THIRD_ROW};
	this->OLED_->PrintString("Pratite uputstva", info3_pos);
	disp_pos_t info4_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::FOURTH_ROW};
	this->OLED_->PrintString("sa terminala", info4_pos);
	this->OLED_->UpdateScreen();
	/* PC display */
	this->PrintUART("****Dobrodosli u Slagalicu****\n***Napomena: Igra je jos uvek u razvoju. ");
	this->PrintUART("Trenutno je moguce igrati igru skocko.\n");
	this->state_ = this->ST_WAITING_PLAYER_NAME;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::SetupUser(void){
	/* Setup player name */
	this->PrintUART("Unesite Vase ime preko UART terminala sa zvezdom na kraju, u formatu: Radovan*\n");
	this->PrintUART("Ako zelite da preskocite ovu opciju samo pritisnite plavi taster na beloj plocici.\n");
	this->ReceivePlayerNameFromUART();
	this->PrintUART("\nVase ime je: ");
	this->PrintPlayerNameToUART();
	this->PrintUART("\n\n");
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::SetupBrightness(void){
	/* Setup OLED brightness */
	this->PrintUART("Podesite kontrast pomeranjem crvenog potenciometra. ");
	HAL_TIM_Base_Start_IT(this->timerOLED_);
	this->PrintUART("Kada ste zadovoljni sa kontrastom, pritisnite plavi taster.\n\n");
	this->state_ = this->ST_WAITING_BRIGHTNESS;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintGameInfo(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	this->PrintUART("\n***UPUTSTVO***\n");
	this->PrintUART("Da biste otpoceli unos pocetne ili bilo koje naredne kombinacije potrebno je da pritisnete bilo koji od sledecih tastera:\n");
	this->PrintUART(" - Plavi taster - nalazi se na beloj plocici\n");
	this->PrintUART(" - Braon taster SW1 - nalazi se na plavoj plocici odmah ispod OLED displeja (levi od dva braon tastera)\n");
	this->PrintUART(" - Braon taster SW2 - nalazi se na plavoj plocici odmah ispod OLED displeja (desni od dva braon tastera)\n");
	this->PrintUART("Na tastere SW2/SW1 se pomerate udesno/ulevo.\nOkretanjem crnog potenciometra birate znak.\n");
	this->PrintUART("Na plavi taster potvrdjujete unetu kombinaciju. Tom prilikom se putem UART-a ispisuje kombinacija koju ste uneli ");
	this->PrintUART("i rezultat koji ste tom prilikom ostvarili.\n");
	this->PrintUART("\nDa biste poceli igru pritisnite bilo koji od tri pomenuta tastera.\n");
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::SetBrightness(const uint32_t& value){
	uint32_t temp;
	temp = value / 16;
	this->OLED_->SetBrightness((uint8_t) temp);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::ConfirmBrightness(void){
	HAL_TIM_Base_Stop_IT(this->timerOLED_);
	//this->SetupUser();
	this->PrintGameInfo();
	this->state_ = this->ST_WAITING_TO_START;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::StartSkocko(void){
	//TODO
	this->PrintUART("\nNova igra je pocela!\nSrecno!!!\n\n\n");
	this->PrintUART("******************************************************************************************\n");
	this->PrintUART("*                                                                                        *\n");
	this->PrintUART("*                                                                                        *\n");
	this->PrintUART("*    |------------------------------------------------------------------------------|    *\n");
	this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
	this->PrintUART("*    ||    S1    |    S2    |    S3    |    S4    |||   Na mestu   | Nisu na mestu ||    *\n");
	this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
	this->PrintUART("*    |                                                                              |    *\n");
	this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
	this->state_ = this->ST_RECEIVING_COMB;
	this->WinCombGen();
	this->OLED_->Clear();
	disp_pos_t header_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::FIRST_COL};
	this->OLED_->PrintString("IGRA SKOCKO", header_pos);
	this->OLED_->UpdateScreen();
	this->SEV_SEG_->Print(1);
	HAL_TIM_Base_Start_IT(this->timerOLED_);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t Skocko::GetState(void){
	return this->state_;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::SetWinFactor(const uint16_t& winFactor){
	this->winFactor_ = winFactor;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::WinCombGen(void){
	HAL_TIM_Base_Stop(this->timerWinCombGen_);
	uint16_t timVal = __HAL_TIM_GET_COUNTER(this->timerWinCombGen_);
	uint32_t temp1 = timVal * this->winFactor_;
	for(uint8_t i = 0; i < 4; i++){
		switch((temp1 & this->mask_[i]) >> (i*4) ){
		case 0:
			this->winComb_[i] = this->SKOCKO;
			break;
		case 1:
			this->winComb_[i] = this->ZVEZDA;
			break;
		case 2:
			this->winComb_[i] = this->PIK;
			break;
		case 3:
			this->winComb_[i] = this->HERC;
			break;
		case 4:
			this->winComb_[i] = this->TREF;
			break;
		case 5:
			this->winComb_[i] = this->KARO;
			break;
		case 6:
			this->winComb_[i] = this->SKOCKO;
			break;
		case 7:
			this->winComb_[i] = this->ZVEZDA;
			break;
		case 8:
			this->winComb_[i] = this->PIK;
			break;
		case 9:
			this->winComb_[i] = this->HERC;
			break;
		case 10:
			this->winComb_[i] = this->TREF;
			break;
		case 11:
			this->winComb_[i] = this->KARO;
			break;
		case 12:
			this->winComb_[i] = this->SKOCKO;
			break;
		case 13:
			this->winComb_[i] = this->ZVEZDA;
			break;
		case 14:
			this->winComb_[i] = this->PIK;
			break;
		case 15:
			this->winComb_[i] = this->HERC;
			break;
		default:
			break;
		}
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::NewComb(void){
	this->currComb_[this->POSITION_1] = this->NONE;
	this->currComb_[this->POSITION_2] = this->NONE;
	this->currComb_[this->POSITION_3] = this->NONE;
	this->currComb_[this->POSITION_4] = this->NONE;
	this->OLED_->Clear();
	disp_pos_t header_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::FIRST_COL};
	this->OLED_->PrintString("IGRA SKOCKO", header_pos);
	this->OLED_->UpdateScreen();
	HAL_TIM_Base_Start_IT(this->timerOLED_);
	this->state_ = this->ST_RECEIVING_COMB;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::IncrementSymbol(const uint8_t& position){
	this->operator +(position);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::IncrementPosition(void){
	if(this->currPos_ < this->POSITION_4){
		this->currPos_++;
	}
	else{
		this->currPos_ = this->POSITION_1;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::DecrementPosition(void){
	if(this->currPos_ > POSITION_1){
		this->currPos_--;
	}
	else{
		this->currPos_ = POSITION_4;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::operator+(const uint8_t& position){
	if(this->currComb_[position] == this->KARO){ this->currComb_[position] = this->SKOCKO; }
	else{ this->currComb_[position]++; }
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
Skocko::combCnt Skocko::CountSymbols(void){
	combCnt comb = {0};

	for(uint8_t i = 0; i < COMB_SYMBOLS_CNT; i++){

		switch(this->winComb_[i]){
		case this->SKOCKO:
			comb.winComb.skockoCnt_++;
			break;
		case this->ZVEZDA:
			comb.winComb.zvezdaCnt_++;
			break;
		case this->PIK:
			comb.winComb.pikCnt_++;
			break;
		case this->HERC:
			comb.winComb.hercCnt_++;
			break;
		case this->TREF:
			comb.winComb.trefCnt_++;
			break;
		case this->KARO:
			comb.winComb.karoCnt_++;
			break;
		default:
			break;
		}

		switch(this->currComb_[i]){
		case this->SKOCKO:
			comb.userComb.skockoCnt_++;
			break;
		case this->ZVEZDA:
			comb.userComb.zvezdaCnt_++;
			break;
		case this->PIK:
			comb.userComb.pikCnt_++;
			break;
		case this->HERC:
			comb.userComb.hercCnt_++;
			break;
		case this->TREF:
			comb.userComb.trefCnt_++;
			break;
		case this->KARO:
			comb.userComb.karoCnt_++;
			break;
		default:
			break;
		}
	}
	return comb;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
Skocko::result Skocko::CalculateResult(void){

	combCnt comb = CountSymbols();
	result res;
	res.half_correct_ = 0;
	res.correct_ = 0;

	if(this->currComb_[this->POSITION_1] == this->winComb_[this->POSITION_1]){
		res.correct_++;
	}
	if(this->currComb_[this->POSITION_2] == this->winComb_[this->POSITION_2]){
		res.correct_++;
	}
	if(this->currComb_[this->POSITION_3] == this->winComb_[this->POSITION_3]){
		res.correct_++;
	}
	if(this->currComb_[this->POSITION_4] == this->winComb_[this->POSITION_4]){
		res.correct_++;
	}

	if(comb.winComb.skockoCnt_ >= comb.userComb.skockoCnt_){ res.half_correct_ += comb.userComb.skockoCnt_; }
	else{ res.half_correct_ += comb.winComb.skockoCnt_; }

	if(comb.winComb.zvezdaCnt_ >= comb.userComb.zvezdaCnt_){ res.half_correct_ += comb.userComb.zvezdaCnt_; }
	else{ res.half_correct_ += comb.winComb.zvezdaCnt_; }

	if(comb.winComb.pikCnt_ >= comb.userComb.pikCnt_){ res.half_correct_ += comb.userComb.pikCnt_; }
	else{ res.half_correct_ += comb.winComb.pikCnt_; }

	if(comb.winComb.hercCnt_ >= comb.userComb.hercCnt_){ res.half_correct_ += comb.userComb.hercCnt_; }
	else{ res.half_correct_ += comb.winComb.hercCnt_; }

	if(comb.winComb.trefCnt_ >= comb.userComb.trefCnt_){ res.half_correct_ += comb.userComb.trefCnt_; }
	else{ res.half_correct_ += comb.winComb.trefCnt_; }

	if(comb.winComb.karoCnt_ >= comb.userComb.karoCnt_){ res.half_correct_ += comb.userComb.karoCnt_; }
	else{ res.half_correct_ += comb.winComb.karoCnt_; }

	res.half_correct_ -= res.correct_;
	return res;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintResult(const result& res){
	//TODO
	/* Message to OLED */
	string message = "Result = ";
	message += std::to_string(res.correct_);
	message += ", ";
	message += std::to_string(res.half_correct_);
	disp_pos_t winMSG = {.x_ = OLED::FIRST_COL, .y_ = OLED::THIRD_ROW};
	this->OLED_->PrintString(message, winMSG);
	this->OLED_->UpdateScreen();

	/* Message to UART */
	this->PrintUART("*    ||");
	PrintCombToUART(this->currComb_);

	this->PrintUART("||      ");
	this->PrintUART(res.correct_);
	this->PrintUART("       |       ");
	this->PrintUART(res.half_correct_);
	this->PrintUART("       ||    *\n");
	this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintWinComb(const uint8_t& dispRow){
	this->OLED_->PrintSkockoComb(this->winComb_, this->currPos_, OLED::FOURTH_ROW, OLED::NO_UNDERLINE);
	this->OLED_->UpdateScreen();
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::RefreshOLED(void){
	this->OLED_->PrintSkockoComb(this->currComb_, this->currPos_, OLED::SECOND_ROW, OLED::WITH_UNDERLINE);
	this->OLED_->UpdateScreen();
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::ConfirmComb(void){
	if (this->InputCheck() != this->INPUT_VALID) {
		return;
	}

	HAL_TIM_Base_Stop_IT(this->timerOLED_);
	this->currTry_++;
	result res = this->CalculateResult();
	this->PrintResult(res);
	if((this->currTry_ == this->MAX_TRIES) || (res.correct_ == COMB_SYMBOLS_CNT)){
		//this->PrintWinComb(OLED::FOURTH_ROW);
		this->EndGame(res);
	}
	else {
		this->SEV_SEG_->Increment();
		this->currPos_ = this->POSITION_1;
		this->currComb_[this->POSITION_1] = this->NONE;
		this->currComb_[this->POSITION_2] = this->NONE;
		this->currComb_[this->POSITION_3] = this->NONE;
		this->currComb_[this->POSITION_4] = this->NONE;
		this->state_ = this->ST_WAITING_NEXT_COMB;

	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::ResetGame(void){
	/* Start timer for WinningCombinationGeneration */
	HAL_TIM_Base_Start(this->timerWinCombGen_);
	this->currPos_ = this->POSITION_1;
	this->winComb_[this->POSITION_1] = this->NONE;
	this->winComb_[this->POSITION_2] = this->NONE;
	this->winComb_[this->POSITION_3] = this->NONE;
	this->winComb_[this->POSITION_4] = this->NONE;
	this->currComb_[this->POSITION_1] = this->NONE;
	this->currComb_[this->POSITION_2] = this->NONE;
	this->currComb_[this->POSITION_3] = this->NONE;
	this->currComb_[this->POSITION_4] = this->NONE;
	this->currTry_ = 0;
	//TODO
	//delete all including winComb, and start timer to be able to generate again new comb on next blue button press
	// and reset all data of class Skocko
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::ReceivePlayerNameFromUART(void){
	uint8_t name_i = 0;
	uint8_t ch[1];
	while(1){
		ch[0] = '!';
		HAL_UART_Receive_IT(this->huart_, ch, 1);
		while(ch[0] == '!'){}
		if(ch[0] == '*'){ break; }
		this->playerName_[name_i] = ch[0];
		name_i++;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintPlayerNameToUART(void){
	uint8_t name_i = 0;
	uint8_t ch[1];
	while(this->playerName_[name_i] != '\0'){
		ch[0] = this->playerName_[name_i];
		HAL_UART_Transmit(this->huart_, ch, 1, 5000);
		name_i++;
	}
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintUART(const string& message){

	//char ch;
	uint8_t message_size = message.size();
	uint8_t message_ascci[350];
	for(uint8_t i = 0; i < message_size; i++){
		//ch = message[i];
		//message_ascci[i] = (uint8_t) ch;
		message_ascci[i] = (uint8_t) message[i];
	}
	HAL_UART_Transmit(this->huart_, message_ascci, message_size, 5000);
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintUART(const uint8_t& digit){

	uint8_t digit_ascci[1];
	digit_ascci[0] = digit + 48;
	HAL_UART_Transmit(this->huart_, digit_ascci, 1, 5000);

}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::SetSymbol(const uint8_t& position, const uint8_t& symbol){
	this->currComb_[position] = symbol;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t* Skocko::GetComb(void){
	return this->currComb_;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t Skocko::GetCurrentPosition(void){
	return this->currPos_;
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t Skocko::GetSymbol(const uint8_t& position){
	return this->currComb_[position];
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
uint8_t Skocko::InputCheck(void){
	if((this->GetSymbol(this->POSITION_1) != this->NONE) && (this->GetSymbol(this->POSITION_2) != this->NONE) &&
			(this->GetSymbol(this->POSITION_3) != this->NONE) && (this->GetSymbol(this->POSITION_4) != this->NONE)){

		return this->INPUT_VALID;
	}

	else { return this->INPUT_INVALID; }
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::EndGame(const result& res){
	this->state_ = this->ST_GAME_END;

	if(res.correct_ == COMB_SYMBOLS_CNT){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		this->PrintUART("*    --------------------------------------------------------------------------------    *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("*    |------------------------------------------------------------------------------|    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    ||                         Dobitna kombinacija je:                            ||    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    |                                                                              |    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    ||"); this->PrintCombToUART(this->winComb_); this->PrintUART("||      4       |       0       ||    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    |------------------------------------------------------------------------------|    *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("******************************************************************************************\n\n");
		this->PrintUART("Pogodili ste!!!\n");
		this->PrintUART("\nIgrac: ");
		this->PrintPlayerNameToUART();
		this->PrintUART("\nAko zelite ponovo da igrate, pritisnite bilo koji taster.\n");

		/* OLED */
		this->OLED_->Clear();
		disp_pos_t text1_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::FIRST_COL};
		this->OLED_->PrintString("Pogodili ste!!!", text1_pos);
		this->OLED_->PrintSkockoComb(this->currComb_, this->currPos_, OLED::SECOND_ROW, OLED::NO_UNDERLINE);
		disp_pos_t text2_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::THIRD_ROW};
		this->OLED_->PrintString("Dobitna komb:", text2_pos);
		this->OLED_->PrintSkockoComb(this->winComb_, this->currPos_, OLED::FOURTH_ROW, OLED::NO_UNDERLINE);
		this->OLED_->UpdateScreen();
	}
	else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		this->PrintUART("*    --------------------------------------------------------------------------------    *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("*    |------------------------------------------------------------------------------|    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    ||                         Dobitna kombinacija je:                            ||    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    |                                                                              |    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    ||"); this->PrintCombToUART(this->winComb_); this->PrintUART("||      4       |       0       ||    *\n");
		this->PrintUART("*    ||----------------------------------------------------------------------------||    *\n");
		this->PrintUART("*    |------------------------------------------------------------------------------|    *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("*                                                                                        *\n");
		this->PrintUART("******************************************************************************************\n\n");
		this->PrintUART("Nazalost niste pogodili.\n");
		this->PrintUART("\nIgrac: ");
		this->PrintPlayerNameToUART();
		this->PrintUART("\nAko zelite ponovo da igrate, pritisnite bilo koji taster.\n");

		/* OLED */
		this->OLED_->Clear();
		disp_pos_t text1_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::FIRST_COL};
		this->OLED_->PrintString("Probajte ponovo?", text1_pos);
		this->OLED_->PrintSkockoComb(this->currComb_, this->currPos_, OLED::SECOND_ROW, OLED::NO_UNDERLINE);
		disp_pos_t text2_pos = {.x_ = OLED::FIRST_COL, .y_ = OLED::THIRD_ROW};
		this->OLED_->PrintString("Dobitna komb:", text2_pos);
		this->OLED_->PrintSkockoComb(this->winComb_, this->currPos_, OLED::FOURTH_ROW, OLED::NO_UNDERLINE);
		this->OLED_->UpdateScreen();
	}
	this->ResetGame();
}


/**
  * @brief
  * @note
  * @param
  * @retval
  */
void Skocko::PrintCombToUART(const uint8_t* comb){
	for(int i = 0; i < COMB_SYMBOLS_CNT; i++){
		switch(comb[i]){
		case this->SKOCKO:
			PrintUART("  SKOCKO  |");
			break;
		case this->ZVEZDA:
			PrintUART("  ZVEZDA  |");
			break;
		case this->PIK:
			PrintUART("   PIK    |");
			break;
		case this->HERC:
			PrintUART("   HERC   |");
			break;
		case this->TREF:
			PrintUART("   TREF   |");
			break;
		case this->KARO:
			PrintUART("   KARO   |");
			break;
		default:
			break;
		}
	}
}
