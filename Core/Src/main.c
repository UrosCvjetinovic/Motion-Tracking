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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "motion_tracking.h"
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
BoardParams board = {
    .orientation = Straight,
    .temperature = 26,
    .cycleNumber = 0u
};

uint8_t sendToUart = 0u;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MapOrientation(void);
void ReadData(void);
void ReadBMI088Data(void);
void ReadBME680Data(void);
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
  MX_I2C2_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */

  InitGraphics();
  InitBMI088();
  InitBME680();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    ReadData();
    UpdateGraphics();
    if (sendToUart)
    {
      SendDataToUart();
    }
    HAL_Delay(100);
  }
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief TODO
  * @retval None
  */
void ReadData(void)
{
  switch(screenManager.current)
  {
    case IdleScreen:
      if (screenManager.previous != IdleScreen)
      {
        board.orientationType = Straight;
        StopBMI088Sensor();
        StopBME680Sensor();
        board.cycleNumber = 0u;
      }
    break;
    case ConnectionStatusScreen:
      if ((screenManager.previous != IdleScreen) && (screenManager.previous != ConnectionStatusScreen))
      {
        board.orientationType = Straight;
        StopBMI088Sensor();
        StopBME680Sensor();
        board.cycleNumber = 0u;
      }
      ReadConnectionStatus(&board.connectionStatus);
    break;
      break;
    case BME680RawDataScreen:
      if ((screenManager.previous == IdleScreen) || (screenManager.previous == ConnectionStatusScreen))
      {
        StopBMI088Sensor();
        board.cycleNumber = 0u;
      }
      StartBME680Sensor();
      ReadBME680Data();
      break;
    case BMI088RawDataScreen:
      if ((screenManager.previous != BMI088RawDataScreen) && (screenManager.previous != OrientationScreen))
      {
        StopBME680Sensor();
        StartBMI088Sensor();
        board.cycleNumber = 0u;
      }
      ReadBMI088Data();
      break;
    case OrientationScreen:
      if ((screenManager.previous != BMI088RawDataScreen) && (screenManager.previous != OrientationScreen))
      {
        StopBME680Sensor();
        StartBMI088Sensor();
        board.cycleNumber = 0u;
      }
      ReadBMI088Data();
      MapOrientation();
      break;
    default:
      screenManager.current = IdleScreen;
      break;
  }
}

/**
  * @brief Read data from sensor BMI088
  * @retval None
  */
void ReadBMI088Data(void)
{
  ReadBMI088Temperature(&board.temperature);
  ReadBMI088Acceleration(&board.acceleration);
  ReadBMI088Orientation(&board.orientation);
  board.cycleNumber++;
}

/**
  * @brief Read data from sensor BME680
  * @retval None
  */
void ReadBME680Data(void)
{
  ReadBME680(&board.temperature, &board.pressure, &board.humidity, &board.gasResistance);
  board.cycleNumber++;
}

/**
  * @brief Using read data map orientation to specific type
  * @retval None
  */
void MapOrientation(void)
{
  board.orientationType = GetNewOrientation(board.orientation.x, board.orientation.y, board.orientation.z, BMI088_DEGREEPERSECOND_THRESHOLD, board.orientationType);
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
