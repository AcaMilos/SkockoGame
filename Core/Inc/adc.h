/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc2;

/* USER CODE BEGIN Private defines */

enum{
	ADC_SKOCKO	= 0x298,
	ADC_ZVEZDA	= 0x530,
	ADC_PIK		= 0x7C9,
	ADC_HERC	= 0xA63,
	ADC_TREF	= 0xCFB,
	ADC_KARO	= 0xF93
};

extern volatile uint32_t ADC_result[2];
extern volatile uint8_t ADC_resultReady;
/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);

/* USER CODE BEGIN Prototypes */
enum {
	RESULT_NOT_READY	= 0x00,
	RESULT_READY		= 0x01
};
enum {
	RED_POT		= 0x00,
	BLACK_POT	= 0x01
};

//class ADC{
//public:
//	ADC(void);
//	~ADC(void);
//	uint32_t* getADCval(void);
//	enum {RESULT_NOT_READY, RESULT_READY};
//	static const uint8_t MAX_ADC = 2;
//private:
//	uint32_t ADC_value_[MAX_ADC];
//	uint8_t status_;
//};
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

