/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string>
using std::string;
#include "skocko.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

Skocko* Skocko::instancePointerSkocko = nullptr;
Skocko* mySkocko = nullptr;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  ButtonPressed = BUTTON_NONE;
  ButtonEvent	= BUTTON_NONE;

  mySkocko = Skocko::GetInstance(&hi2c2);

  mySkocko->Init(&htim3, &htim5, &huart2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void UpdateBrightnessFromADC(void){
	uint32_t adcRes = ADC_result[RED_POT];
	mySkocko->SetBrightness(adcRes);
}


void UpdateSkockoSymbolFromADC(void){
	string symbol;
	if(ADC_result[BLACK_POT] < ADC_SKOCKO){
		mySkocko->SetSymbol(mySkocko->GetCurrentPosition(), mySkocko->SKOCKO);
	}
	else if(ADC_result[BLACK_POT] < ADC_ZVEZDA){
		mySkocko->SetSymbol(mySkocko->GetCurrentPosition(), mySkocko->ZVEZDA);
	}
	else if(ADC_result[BLACK_POT] < ADC_PIK){
		mySkocko->SetSymbol(mySkocko->GetCurrentPosition(), mySkocko->PIK);
	}
	else if(ADC_result[BLACK_POT] < ADC_HERC){
		mySkocko->SetSymbol(mySkocko->GetCurrentPosition(), mySkocko->HERC);
	}
	else if(ADC_result[BLACK_POT] < ADC_TREF){
		mySkocko->SetSymbol(mySkocko->GetCurrentPosition(), mySkocko->TREF);
	}
	else{
		mySkocko->SetSymbol(mySkocko->GetCurrentPosition(), mySkocko->KARO);
	}
}


void Timer3Callback(void){
	if(mySkocko->GetState() == mySkocko->ST_WAITING_BRIGHTNESS){
		/* If we are in state of adjusting brightness, then we use this timer for refreshing OLED brightness
		 * so user can see and choose which brightness is the most acceptable to him. */
		HAL_ADC_Start_IT(&hadc1);
	}
	else if (mySkocko->GetState() == mySkocko->ST_RECEIVING_COMB){
		/* In other situation, we use this timer for refreshing OLED content, on which is displayed current
		 * combination which user are choosing. */
		HAL_ADC_Start_IT(&hadc2);
		mySkocko->RefreshOLED();
	}
}


void ButtonPressedEventCallback(void){
	if ((ButtonPressed & BLUE_BUTTON) == BLUE_BUTTON){
		ButtonPressed &= ~(BLUE_BUTTON);
		uint8_t skockoState = mySkocko->GetState();
		switch(skockoState){
		case Skocko::ST_WAITING_PLAYER_NAME:
			mySkocko->SetupBrightness();
			break;
		case Skocko::ST_WAITING_BRIGHTNESS:
			mySkocko->ConfirmBrightness();
			break;
		case Skocko::ST_WAITING_TO_START:
			mySkocko->StartSkocko();
			break;
		case Skocko::ST_RECEIVING_COMB:
			mySkocko->ConfirmComb();
			break;
		case Skocko::ST_WAITING_NEXT_COMB:
			mySkocko->NewComb();
			break;
		case Skocko::ST_GAME_END:
			mySkocko->PrintGameInfo();
			mySkocko->StartSkocko();
			break;
		default:
			break;
		}
	}
	else if((ButtonPressed & BUTTON_1) == BUTTON_1){
		ButtonPressed &= ~(BUTTON_1);
		uint8_t skockoState = mySkocko->GetState();
		switch(skockoState){
		case Skocko::ST_WAITING_PLAYER_NAME:
			mySkocko->SetupBrightness();
			break;
		case Skocko::ST_WAITING_BRIGHTNESS:
			mySkocko->ConfirmBrightness();
			break;
		case Skocko::ST_WAITING_TO_START:
			mySkocko->StartSkocko();
			break;
		case Skocko::ST_RECEIVING_COMB:
			mySkocko->DecrementPosition();
			break;
		case Skocko::ST_WAITING_NEXT_COMB:
			mySkocko->NewComb();
			break;
		case Skocko::ST_GAME_END:
			mySkocko->PrintGameInfo();
			mySkocko->StartSkocko();
			break;
		default:
			break;
		}
	}
	else if((ButtonPressed & BUTTON_2) == BUTTON_2){
		ButtonPressed &= ~(BUTTON_2);
		uint8_t skockoState = mySkocko->GetState();
		switch(skockoState){
		case Skocko::ST_WAITING_PLAYER_NAME:
			mySkocko->SetupBrightness();
			break;
		case Skocko::ST_WAITING_BRIGHTNESS:
			mySkocko->ConfirmBrightness();
			break;
		case Skocko::ST_WAITING_TO_START:
			mySkocko->StartSkocko();
			break;
		case Skocko::ST_RECEIVING_COMB:
			mySkocko->IncrementPosition();
			break;
		case Skocko::ST_WAITING_NEXT_COMB:
			mySkocko->NewComb();
			break;
		case Skocko::ST_GAME_END:
			mySkocko->PrintGameInfo();
			mySkocko->StartSkocko();
			break;
		default:
			break;
		}
	}
	else{
		return;
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
