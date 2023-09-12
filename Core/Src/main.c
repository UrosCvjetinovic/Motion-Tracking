/* USER CODE BEGIN Header */
/**
  *  Created on: Sep 10, 2023
  *      Author: Uros Cvjetinovic
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "bmi08_defs.h"
#include "bmm150_defs.h"
#include "bme680_defs.h"
#include "lookup_table.h"
#include "graphics.h"
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t timeout = 1000u;
  uint16_t size = 1u;
  uint16_t id_BME680 = 0u; // 0xD0
  uint16_t id_BMI088_acc = 0u; // 0x1E acc, 0x0F gyro
  uint16_t id_BMI088_gyro = 0u; // 0x1E acc, 0x0F gyro
  uint16_t id_BMM150 = 0u; //0x32

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_DAC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_ADC1_Init();
  MX_TIM17_Init();
  MX_TIM6_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  Init_Graphics();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t counter = 0u;
  uint8_t sizeImg = 50u;

  while (1)
  {
	  ssd1306_Fill(Black);
	  Update_Graphics();
	  switch(counter) {
	  	  case 0:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgStraight, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 1:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgUp, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 2:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgDown, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 3:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgLeft, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 4:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgRight, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 5:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgUpRight, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 6:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgUpLeft, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 7:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgDownRight, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  	  case 8:
	  		ssd1306_DrawBitmap(0u,0u, &planeImgDownLeft, sizeImg, sizeImg, White);
	  	  Update_Graphics();
	  		break;
	  }
	  counter++;
	  HAL_I2C_Mem_Read(&hi2c2, BMM150_DEFAULT_I2C_ADDRESS << 1u, BMM150_REG_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &id_BMM150, size, HAL_MAX_DELAY);
	  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &id_BMI088_acc, size, HAL_MAX_DELAY);
	  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &id_BMI088_gyro, size, HAL_MAX_DELAY);
	  HAL_I2C_Mem_Read(&hi2c2, BME680_I2C_ADDR_PRIMARY << 1u, BME680_CHIP_ID_ADDR, I2C_MEMADD_SIZE_8BIT, &id_BME680, size, HAL_MAX_DELAY);
	  if (counter > 8u)
		  counter = 0u;
	  HAL_Delay(500);
	  Update_Graphics();
	  //HAL_Delay(10000u);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
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

/* USER CODE BEGIN 4 */
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
