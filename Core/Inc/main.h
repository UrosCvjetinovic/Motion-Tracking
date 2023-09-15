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

#define STM32L476xx

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "13DOF_click.h"
#include "motion_tracking.h"
#include "ssd1306_conf.h"
#include "stdint.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum ScreenTypesEnumeration {
  Idle  = 0,
  ConnectionStatus  = 1,
  RawData  = 2,
  FlightSimulation  = 3,
  ScreenTypes_EnumCount = 4
} ScreenTypes;


typedef struct BoardParams {
  SensorConnectionStruct connectionStatus;
  OrientationTypes orientationType;
  struct bmi08_sensor_data acceleration;
  struct bmi08_sensor_data orientation;
  uint16_t temperature;
} BoardParams;

extern BoardParams board;
extern ScreenTypes previousScreen;
extern ScreenTypes currentScreen;
extern ScreenTypes nextScreen;

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
#define OLED_I2C hi2c2
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define ADC1_POT0_Pin GPIO_PIN_0
#define ADC1_POT0_GPIO_Port GPIOA
#define ADC1_POT1_Pin GPIO_PIN_1
#define ADC1_POT1_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_8
#define SW2_GPIO_Port GPIOC
#define SW2_EXTI_IRQn EXTI9_5_IRQn
#define SW1_Pin GPIO_PIN_9
#define SW1_GPIO_Port GPIOC
#define SW1_EXTI_IRQn EXTI9_5_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
