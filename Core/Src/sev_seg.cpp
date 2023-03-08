/*
 * sev_seg.cpp
 *
 *  Created on: 10 Jan 2023
 *      Author: ACA
 */

#include <sev_seg.h>


SEV_SEG::SEV_SEG(void){
	this->muxTIM_ = new TIM_HandleTypeDef;
	this->currDisp_ = this->DISPLAY_1;
	this->data1_ = this->LETTER_NONE;
	this->data2_ = this->LETTER_NONE;
	this->init_ = this->SEV_SEG_INITIALIZED;
	this->status_ = this->SEV_SEG_OFF;
}



SEV_SEG* SEV_SEG::GetInstance(void){
	if(instancePointerSEV_SEG == nullptr){
		instancePointerSEV_SEG = new SEV_SEG;
		return instancePointerSEV_SEG;
	}
	else{
		return instancePointerSEV_SEG;
	}
}



SEV_SEG::~SEV_SEG(void){
	delete this->muxTIM_;
}


uint8_t SEV_SEG::Init(TIM_HandleTypeDef* htimX, uint8_t data1, uint8_t data2){
	this->currDisp_ = this->DISPLAY_1;
	this->data1_ = data1;
	this->data2_ = data2;
	this->init_ = this->SEV_SEG_INITIALIZED;
	HAL_TIM_Base_Start_IT(htimX);
	this->UpdateDisplay();
	return this->SEV_SEG_OK;
}


void SEV_SEG::Print(uint8_t data){
	this->status_ = this->SEV_SEG_ONE_ACTIVE;
	this->data2_ = data;
	this->currDisp_ = this->DISPLAY_2;
	this->UpdateDisplay();
}

uint8_t SEV_SEG::Clear(void){
	//TODO
	return this->SEV_SEG_OK;
}

uint8_t SEV_SEG::TurnOFF(void){
	//TODO
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState) this->DISPLAY_OFF);	// mux1 = ON
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, (GPIO_PinState) this->DISPLAY_OFF);	// mux2 = OFF
	this->status_ = this->SEV_SEG_OFF;
	return this->SEV_SEG_OK;
}

uint8_t SEV_SEG::TurnON(){
	//TODO
	this->status_ = this->SEV_SEG_BOOTH_ACTIVE;
	this->UpdateDisplay();
	return this->SEV_SEG_OK;
}

uint8_t SEV_SEG::UpdateDisplay(void){
	if(this->status_ != this->SEV_SEG_OFF){
		/* Turn OFF both displays */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState) this->DISPLAY_OFF);	// mux1 = OFF
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, (GPIO_PinState) this->DISPLAY_OFF);	// mux2 = OFF

		if(this->currDisp_ == DISPLAY_1){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (GPIO_PinState) sa[this->data1_]);	// a
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  (GPIO_PinState) sb[this->data1_]);	// b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  (GPIO_PinState) sc[this->data1_]);	// c
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState) sd[this->data1_]);	// d
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (GPIO_PinState) se[this->data1_]);	// e
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (GPIO_PinState) sf[this->data1_]);	// f
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  (GPIO_PinState) sg[this->data1_]);	// g
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,  (GPIO_PinState) this->DISPLAY_ON);	// mux1 = ON
		}
		else if (this->currDisp_ == DISPLAY_2){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (GPIO_PinState) sa[this->data2_]);	// a
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  (GPIO_PinState) sb[this->data2_]);	// b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  (GPIO_PinState) sc[this->data2_]);	// c
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState) sd[this->data2_]);	// d
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (GPIO_PinState) se[this->data2_]);	// e
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (GPIO_PinState) sf[this->data2_]);	// f
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  (GPIO_PinState) sg[this->data2_]);	// g
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,  (GPIO_PinState) this->DISPLAY_ON);	// mux2 = ON
		}
		else{
			return this->SEV_SEG_OK;	// display is OFF so we just should exit
		}
	}

	return this->SEV_SEG_OK;
}

uint8_t SEV_SEG::SwitchDisplay(void){
	if(this->status_ != this->SEV_SEG_OFF){
		/* Turn OFF both displays */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState) DISPLAY_OFF);	// mux1 = OFF
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, (GPIO_PinState) DISPLAY_OFF);	// mux2 = OFF

		if(this->currDisp_ == DISPLAY_1){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (GPIO_PinState) sa[this->data2_]);	// a
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  (GPIO_PinState) sb[this->data2_]);	// b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  (GPIO_PinState) sc[this->data2_]);	// c
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState) sd[this->data2_]);	// d
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (GPIO_PinState) se[this->data2_]);	// e
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (GPIO_PinState) sf[this->data2_]);	// f
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  (GPIO_PinState) sg[this->data2_]);	// g
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,  (GPIO_PinState) this->DISPLAY_ON);	// mux2 = ON
			this->currDisp_ = DISPLAY_2;
		}
		else if (this->currDisp_ == DISPLAY_2){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (GPIO_PinState) sa[this->data1_]);	// a
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  (GPIO_PinState) sb[this->data1_]);	// b
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  (GPIO_PinState) sc[this->data1_]);	// c
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState) sd[this->data1_]);	// d
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (GPIO_PinState) se[this->data1_]);	// e
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (GPIO_PinState) sf[this->data1_]);	// f
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  (GPIO_PinState) sg[this->data1_]);	// g
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,  (GPIO_PinState) this->DISPLAY_ON);	// mux1 = ON
			this->currDisp_ = DISPLAY_1;
		}
		else{
			return this->SEV_SEG_OK;	// display is OFF so we just should exit
		}
	}
	return this->SEV_SEG_OK;
}

void SEV_SEG::Increment (void){
	if(this->status_ == this->SEV_SEG_ONE_ACTIVE){
		this->data2_++;
	}
	else if(this->status_ == this->SEV_SEG_BOOTH_ACTIVE){
		this->data2_++;
		if (this->data2_ == 0) {
			this->data1_++;
		}
	}
	else { return; }

	this->UpdateDisplay();
}
