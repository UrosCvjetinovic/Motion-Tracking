/*
 * graphics.c
 *
 *  Created on: Sep 10, 2023
 *      Author: Uros Cvjetinovic
 */
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "main.h"
#include "ssd1306.h"

void InitGraphics(void)
{
	ssd1306_Init();

	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_Fill(Black);

	ssd1306_UpdateScreen();

  SetScreen(Idle);
}

void SetScreen(ScreenTypes screen)
{
  if (screen < ScreenTypes_EnumCount)
  {
    nextScreen = screen;
  }
}

void PreviousScreen(void)
{
  if (nextScreen == Idle)
  {
    nextScreen = ScreenTypes_EnumCount - 1u;
  }
  else
  {
    nextScreen--;
  }
}

void NextScreen(void)
{
  nextScreen++;
  if (nextScreen >= ScreenTypes_EnumCount)
  {
    nextScreen = Idle;
  }
}

void UpdateGraphics(void)
{
  ssd1306_Fill(Black);

  switch(currentScreen)
  {
    case Idle:
      DisplayIdleMenu();
      ssd1306_UpdateScreen();
      break;
    case ConnectionStatus:
      DisplayConnectionStatus();
      ssd1306_UpdateScreen();
      break;
    case RawData:
      DisplayRawData();
      ssd1306_UpdateScreen();
      break;
    case FlightSimulation:
      DisplayOrientation();
      ssd1306_UpdateScreen();
      break;
    default:
      currentScreen = Idle;
      break;
  }
  previousScreen = currentScreen;
  currentScreen = nextScreen;
}

void DisplayIdleMenu(void)
{
  uint8_t xOffsetScreenName = 0u;
  uint8_t yOffsetScreenName = 20u;

  ssd1306_Fill(Black);

  ssd1306_SetCursor(0u, 0u);
  ssd1306_WriteString("Motion Tracking", Font_7x10, White);

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*ConnectionStatus", Font_7x10, White);
  yOffsetScreenName += 10u;

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*Raw data", Font_7x10, White);
  yOffsetScreenName += 10u;

  ssd1306_SetCursor(xOffsetScreenName, yOffsetScreenName);
  ssd1306_WriteString("*Orientation", Font_7x10, White);
  yOffsetScreenName += 10u;
}

void DisplayConnectionStatus(void)
{
  uint8_t XOffsetSensorName = 0u;
  uint8_t YOffsetSensorName = 10u;
  uint8_t XOffsetSensorStatus = 70u;
  uint8_t YOffsetSensorStatus = 10u;

  ssd1306_Fill(Black);

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

void DisplayRawData(void)
{
  uint8_t XOffsetSensorName = 0u;
  uint8_t YOffsetSensorName = 10u;
  uint8_t XOffsetSensorStatus = 40u;
  uint8_t YOffsetSensorStatus = 10u;

  ssd1306_Fill(Black);

  ssd1306_SetCursor(30u, 0u);
  ssd1306_WriteString("Raw data:", Font_7x10, White);

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
  ssd1306_Fill(Black);
  ssd1306_DrawBitmap(IMAGE_PLANE_POSITION_X, IMAGE_PLANE_POSITION_Y, planeImgs[board.orientationType], IMAGE_PLANE_SIZE, IMAGE_PLANE_SIZE, White);

}

void WriteNumber(int32_t number, uint8_t numberOfDigits, uint8_t x, uint8_t y)
{
  const uint8_t charShift = 7u;
  uint8_t decimal;
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
      decimal = number % 10u;
      ssd1306_WriteChar(decimal+'0', Font_7x10, White);
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
