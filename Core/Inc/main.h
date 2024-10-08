/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI1_CS1_Pin GPIO_PIN_0
#define SPI1_CS1_GPIO_Port GPIOB
#define SPI1_CS2_Pin GPIO_PIN_1
#define SPI1_CS2_GPIO_Port GPIOB
#define ButtonF1_Pin GPIO_PIN_8
#define ButtonF1_GPIO_Port GPIOA
#define ButtonLeft_Pin GPIO_PIN_9
#define ButtonLeft_GPIO_Port GPIOA
#define ButtonDown_Pin GPIO_PIN_10
#define ButtonDown_GPIO_Port GPIOA
#define ButtonMid_Pin GPIO_PIN_11
#define ButtonMid_GPIO_Port GPIOA
#define ButtonUp_Pin GPIO_PIN_12
#define ButtonUp_GPIO_Port GPIOA
#define ButtonF2_Pin GPIO_PIN_6
#define ButtonF2_GPIO_Port GPIOB
#define ButtonRight_Pin GPIO_PIN_7
#define ButtonRight_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
