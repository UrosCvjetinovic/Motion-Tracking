/*
 * graphics.c
 *
 *  Created on: Sep 10, 2023
 *      Author: Uros Cvjetinovic
 */
#include <stdlib.h>
#include <string.h>

#include "usart.h"
#include "graphics.h"
#include "main.h"
#include "ssd1306.h"


#define IMAGE_PLANE_POSITION_X ((uint8_t) 35u)
#define IMAGE_PLANE_POSITION_Y ((uint8_t) 14u)

void WriteNumber(int32_t number, uint8_t numberOfDigits, uint8_t x, uint8_t y);
void DisplayIdleMenu(void);
void DisplayConnectionStatus(void);
void DisplayBME680RawData(void);
void DisplayBMI088RawData(void);
void DisplayOrientation(void);

ScreenManager screenManager = {ScreenTypes_EnumCount, ScreenTypes_EnumCount, IdleScreen};

void InitGraphics(void)
{
	ssd1306_Init();

	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_Fill(Black);

	ssd1306_UpdateScreen();

  SetScreen(IdleScreen);
}

void SetScreen(ScreenTypes screen)
{
  if (screen < ScreenTypes_EnumCount)
  {
    screenManager.next = screen;
  }
}

void PreviousScreen(void)
{
  if (screenManager.next == IdleScreen)
  {
    screenManager.next = ScreenTypes_EnumCount - 1u;
  }
  else
  {
    screenManager.next--;
  }
}

void NextScreen(void)
{
  screenManager.next++;
  if (screenManager.next >= ScreenTypes_EnumCount)
  {
    screenManager.next = IdleScreen;
  }
}

void UpdateGraphics(void)
{
  ssd1306_Fill(Black);

  switch(screenManager.current)
  {
    case IdleScreen:
      DisplayIdleMenu();
      ssd1306_UpdateScreen();
      break;
    case ConnectionStatusScreen:
      DisplayConnectionStatus();
      ssd1306_UpdateScreen();
      break;
    case BME680RawDataScreen:
      DisplayBME680RawData();
      ssd1306_UpdateScreen();
      break;
    case BMI088RawDataScreen:
      DisplayBMI088RawData();
      ssd1306_UpdateScreen();
      break;
    case OrientationScreen:
      DisplayOrientation();
      ssd1306_UpdateScreen();
      break;
    default:
      screenManager.current = IdleScreen;
      break;
  }
  screenManager.previous = screenManager.current;
  screenManager.current = screenManager.next;
}

void DisplayIdleMenu(void)
{
  uint8_t xOffsetScreenName = 0u;
  uint8_t yOffsetScreenName = 20u;

  ssd1306_SetCursor(0u, 0u);
  ssd1306_WriteString("Motion Tracking", Font_7x10, White);

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*ConnectionStatus", Font_7x10, White);
  yOffsetScreenName += 10u;

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*Raw data BME680", Font_7x10, White);
  yOffsetScreenName += 10u;

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*Raw data BMI088", Font_7x10, White);
  yOffsetScreenName += 10u;

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*Orientation", Font_7x10, White);
  yOffsetScreenName += 10u;
}

void DisplayConnectionStatus(void)
{
  uint8_t XOffsetSensorName = 0u;
  uint8_t YOffsetSensorName = 15u;
  uint8_t XOffsetSensorStatus = 60u;
  uint8_t YOffsetSensorStatus = 15u;

  ssd1306_SetCursor(30u, 0u);
  ssd1306_WriteString("Connected:", Font_7x10, White);

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("BMM150", Font_7x10, White);
  if (board.connectionStatus.BMM150)
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- YES", Font_7x10, White);
  }
  else
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- NO", Font_7x10, White);
  }
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("BMI08_A", Font_7x10, White);
  if (board.connectionStatus.BMI088_acc)
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- YES", Font_7x10, White);
  }
  else
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- NO", Font_7x10, White);
  }
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("BMI08_G", Font_7x10, White);
  if (board.connectionStatus.BMI088_gyro)
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- YES", Font_7x10, White);
  }
  else
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- NO", Font_7x10, White);
  }
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("BME680", Font_7x10, White);
  if (board.connectionStatus.BME680)
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- YES", Font_7x10, White);
  }
  else
  {
    ssd1306_SetCursor(XOffsetSensorStatus, YOffsetSensorStatus);
    ssd1306_WriteString("- NO", Font_7x10, White);
  }
}

void DisplayBME680RawData(void)
{
  uint8_t XOffsetSensorName = 0u;
  uint8_t YOffsetSensorName = 10u;
  uint8_t XOffsetSensorStatus = 70u;
  uint8_t YOffsetSensorStatus = 10u;

  ssd1306_SetCursor(10u, 0u);
  ssd1306_WriteString("BME680 Raw data:", Font_7x10, White);

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("T [C]:", Font_7x10, White);
  WriteNumber(board.temperature, 2u, XOffsetSensorStatus, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("P [kPa]", Font_7x10, White);
  WriteNumber(board.pressure, 3u, XOffsetSensorStatus, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("Hum [%]", Font_7x10, White);
  WriteNumber(board.humidity, 3u, XOffsetSensorStatus, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("GR [kOhm]", Font_7x10, White);
  WriteNumber(board.gasResistance, 3u, XOffsetSensorStatus, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;
}

void DisplayBMI088RawData(void)
{
  uint8_t XOffsetSensorName = 0u;
  uint8_t YOffsetSensorName = 10u;
  uint8_t XOffsetSensorStatus = 40u;
  uint8_t YOffsetSensorStatus = 10u;

  ssd1306_SetCursor(10u, 0u);
  ssd1306_WriteString("BMI088 Raw data:", Font_7x10, White);

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("T [C]:", Font_7x10, White);
  WriteNumber(board.temperature, 2u, XOffsetSensorStatus, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName + 30u, YOffsetSensorName);
  ssd1306_WriteString("mg:", Font_7x10, White);
  ssd1306_SetCursor(XOffsetSensorName + 70u, YOffsetSensorName);
  ssd1306_WriteString("DegPerS:", Font_7x10, White);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("X:", Font_7x10, White);
  WriteNumber(board.acceleration.x, 3u, XOffsetSensorStatus, YOffsetSensorStatus);
  WriteNumber(board.orientation.x, 3u, XOffsetSensorStatus + 40u, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("Y:", Font_7x10, White);
  WriteNumber(board.acceleration.y, 3u, XOffsetSensorStatus, YOffsetSensorStatus);
  WriteNumber(board.orientation.y, 3u, XOffsetSensorStatus + 40u, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;

  ssd1306_SetCursor(XOffsetSensorName, YOffsetSensorName);
  ssd1306_WriteString("Z:", Font_7x10, White);
  WriteNumber(board.acceleration.z, 3u, XOffsetSensorStatus, YOffsetSensorStatus);
  WriteNumber(board.orientation.z, 3u, XOffsetSensorStatus + 40u, YOffsetSensorStatus);
  YOffsetSensorName += 10u;
  YOffsetSensorStatus += 10u;
}

void DisplayOrientation(void)
{
  ssd1306_SetCursor(0u, 0u);
  ssd1306_WriteString(OrientationTypeNames[board.orientationType], Font_7x10, White);
  ssd1306_DrawBitmap(IMAGE_PLANE_POSITION_X, IMAGE_PLANE_POSITION_Y, planeImgs[board.orientationType], IMAGE_PLANE_SIZE, IMAGE_PLANE_SIZE, White);

}

void WriteNumber(int32_t number, uint8_t numberOfDigits, uint8_t x, uint8_t y)
{
  const uint8_t charShift = 7u;
  uint8_t digits;
  uint8_t sign = '+';

  ssd1306_SetCursor(x, y);
  if (number < 0)
  {
    sign = '-';
    number = -number;
  }

  if (number != 0)
  {
    x += numberOfDigits*charShift + 1;
    ssd1306_SetCursor(x, y);
    while (number > 0u)
    {
      digits = number % 10u;
      ssd1306_WriteChar(digits+'0', Font_7x10, White);
      number = number / 10;
      x -= charShift;
      ssd1306_SetCursor(x, y);
    }
    ssd1306_WriteChar(sign, Font_7x10, White);
  }
  else
  {
    x += numberOfDigits*charShift + 1;
    ssd1306_SetCursor(x, y);
    ssd1306_WriteChar('0', Font_7x10, White);
  }
}

void my_itoa(int32_t number, char* text, uint32_t *length)
{
  *length = 0u;
  uint8_t digits[10];
  uint8_t numberOfDigits = 0u;
  uint8_t sign = '+';

  if (number < 0)
  {
    sign = '-';
    number = -number;
  }

  if (number != 0)
  {
    while (number > 0u)
    {
      digits[numberOfDigits++] = number % 10u + '0';
      number = number / 10;
    }
  }
  else
  {
    digits[numberOfDigits++] = '0';
  }

  *length = 1u + numberOfDigits;
  text[0] = sign;
  for (uint8_t i = 1u; numberOfDigits > 0u; numberOfDigits--)
  {
    text[i++] = digits[numberOfDigits-1u];
  }

}


void SendDataToUart(void)
{
  uint32_t TimeOut = HAL_MAX_DELAY;
  uint32_t msgLength = 0u;
  uint8_t CurrentScreenText[20] = "Current screen: ";
  uint8_t currentScreen[20];
  uint8_t TemperatureText[20] = "T [C]:";
  uint8_t TemperatureValueText[20];
  my_itoa(screenManager.current, currentScreen, &msgLength);
  uint8_t NewLine[20] = "\n";
  HAL_UART_Transmit(&huart2, (uint8_t*)CurrentScreenText, 18u, TimeOut );
  HAL_Delay(100u);
  HAL_UART_Transmit(&huart2, (uint8_t*)currentScreen, msgLength, TimeOut );
  HAL_Delay(100u);
  HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
  HAL_Delay(100u);

  switch(screenManager.current)
  {
    case ConnectionStatusScreen:
      uint8_t ConnectedText[20] = "Connected\n";
      uint8_t BMM150Text[20] = "BMM150";
      uint8_t BMI08_AText[20] = "BMI08_A";
      uint8_t BMI08_GText[20] = "BMI08_G";
      uint8_t BME680Text[20] = "BME680";
      uint8_t yesText[20] = "- YES\n";
      uint8_t noText[20] = "- NO\n";

      HAL_UART_Transmit(&huart2, (uint8_t*)ConnectedText, 12u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)BMM150Text, 6u, TimeOut );
      HAL_Delay(100u);
      if (board.connectionStatus.BMM150)
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)yesText, 7u, TimeOut );
      }
      else
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)noText, 7u, TimeOut );
      }
      HAL_Delay(100u);

      HAL_UART_Transmit(&huart2, (uint8_t*)BMI08_AText, 7u, TimeOut );
      HAL_Delay(100u);
      if (board.connectionStatus.BMI088_acc)
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)yesText, 7u, TimeOut );
      }
      else
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)noText, 7u, TimeOut );
      }
      HAL_Delay(100u);

      HAL_UART_Transmit(&huart2, (uint8_t*)BMI08_GText, 7u, TimeOut );
      HAL_Delay(100u);
      if (board.connectionStatus.BMI088_gyro)
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)yesText, 7u, TimeOut );
      }
      else
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)noText, 7u, TimeOut );
      }
      HAL_Delay(100u);

      HAL_UART_Transmit(&huart2, (uint8_t*)BME680Text, 7u, TimeOut );
      HAL_Delay(100u);
      if (board.connectionStatus.BME680)
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)yesText, 7u, TimeOut );
      }
      else
      {
        HAL_UART_Transmit(&huart2, (uint8_t*)noText, 7u, TimeOut );
      }
      HAL_Delay(100u);
      break;
    case BME680RawDataScreen:
      uint8_t BME680RawDataText[20] = "BME680 Raw data:\n";
      uint8_t PresureText[20] = "P [kPa]:  ";
      uint8_t HumidityText[20] = "Hum [%]:  ";
      uint8_t PresureValueText[20];
      uint8_t HumidityValueText[20];

      HAL_UART_Transmit(&huart2, (uint8_t*)BME680RawDataText, 17u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.temperature, TemperatureValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)TemperatureText, 7u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)TemperatureValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.pressure, PresureValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)PresureText, 7u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)PresureValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.humidity, HumidityValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)HumidityText, 7u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)HumidityValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);
      break;
    case BMI088RawDataScreen:
      uint8_t BMI088RawDataText[20] = "BMI088 Raw data:\n";
      uint8_t AccelerationXText[20] = "Acc[cm/s^2] x:  ";
      uint8_t AccelerationYText[20] = "Acc[cm/s^2] y:  ";
      uint8_t AccelerationZText[20] = "Acc[cm/s^2] z:  ";
      uint8_t AccelerationXValueText[20];
      uint8_t AccelerationYValueText[20];
      uint8_t AccelerationZValueText[20];
      uint8_t OrientationXText[20] = "DegPerS x:  ";
      uint8_t OrientationYText[20] = "DegPerS y:  ";
      uint8_t OrientationZText[20] = "DegPerS z:  ";
      uint8_t OrientationXValueText[20];
      uint8_t OrientationYValueText[20];
      uint8_t OrientationZValueText[20];

      HAL_UART_Transmit(&huart2, (uint8_t*)BMI088RawDataText, 17u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.temperature, TemperatureValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)TemperatureText, 7u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)TemperatureValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.acceleration.x, AccelerationXValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)AccelerationXText, 16u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)AccelerationXValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.acceleration.y, AccelerationYValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)AccelerationYText, 16u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)AccelerationYValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.acceleration.z, AccelerationZValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)AccelerationZText, 16u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)AccelerationZValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.orientation.x, OrientationXValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationXText, 12u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationXValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);

      my_itoa(board.orientation.y, OrientationYValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationYText, 12u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationYValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, 100u );
      HAL_Delay(100u);

      my_itoa(board.orientation.z, OrientationZValueText, &msgLength);
      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationZText, 12u, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationZValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);
      break;
    case OrientationScreen:
      uint8_t OrientationText[20] = "Orientation:\n";
      uint8_t OrientationValueText[20];
      my_itoa(board.orientationType, OrientationValueText, &msgLength);

      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationTypeNames[board.orientationType], 15u, TimeOut );
      HAL_Delay(100u);

      HAL_UART_Transmit(&huart2, (uint8_t*)OrientationValueText, msgLength, TimeOut );
      HAL_Delay(100u);
      HAL_UART_Transmit(&huart2, (uint8_t*)NewLine, 2u, TimeOut );
      HAL_Delay(100u);
      break;
    default:
      break;
  }
  uint8_t oldOutput[20] = "------------------\n";
  HAL_UART_Transmit(&huart2, (uint8_t*)oldOutput,  10u, TimeOut );
}
