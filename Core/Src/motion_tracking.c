/* USER CODE BEGIN Header */
/**
  *  Created on: Sep 15, 2023
  *      Author: Uros Cvjetinovic
  ******************************************************************************
  * @file           : motion_tracking.c
  * @brief          : Logic responsible for tracking the motion
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "motion_tracking.h"

/* USER CODE END Includes */
const char* OrientationTypeNames[] =
{
  "Straight", "StraightFlipped", "StraightLeft", "StraightRight",
  "StraightBack", "StraightBackFlipped", "StraightBackLeft", "StraightBackRight",
  "Up", "Down", "Left", "Right",
  "UpBack", "DownBack", "LeftBack", "RightBack",
  "SideLeft", "SideRight", "SideLeftFlipped", "SideRightFlipped",
  "SideLeftUp", "SideRightUp", "SideLeftDown", "SideRightDown"
};

const uint8_t xPositiveTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    StraightRight, StraightLeft, StraightFlipped, Straight,
    StraightBackLeft, StraightBackRight, StraightBack, StraightBackFlipped,
    SideRightUp, SideRightDown, SideLeftFlipped, SideRight,
    SideLeftUp, SideLeftDown, SideLeft, SideRightFlipped,
    Left, RightBack, LeftBack, Right,
    Up, UpBack, Down, DownBack
};
const uint8_t xNegativeTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    StraightLeft, StraightRight, Straight, StraightFlipped,
    StraightBackRight, StraightBackLeft, StraightBackFlipped, StraightBack,
    SideLeftUp, SideLeftDown, SideLeft, SideRightFlipped,
    SideRightUp, SideRightDown, SideLeftFlipped, SideRight,
    LeftBack, Right, Left, RightBack,
    UpBack, Up, DownBack, Down
};
const uint8_t yPositiveTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    DownBack, UpBack, RightBack, LeftBack,
    Down, Up, Right, Left,
    Straight, StraightFlipped, StraightLeft, StraightRight,
    StraightBack, StraightBackFlipped, StraightBackLeft, StraightBackRight,
    SideLeftDown, SideRightDown, SideRightUp, SideLeftUp,
    SideLeft, SideRight, SideRightFlipped, SideLeftFlipped
};
const uint8_t yNegativeTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    Up, Down, Left, Right,
    UpBack, DownBack, LeftBack, RightBack,
    StraightBackFlipped, StraightBack, StraightBackRight, StraightBackLeft,
    StraightFlipped, Straight, StraightRight, StraightLeft,
    SideLeftUp, SideRightUp, SideRightDown, SideLeftDown,
    SideRightFlipped, SideLeftFlipped, SideLeft, SideRight
};
const uint8_t zPositiveTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    SideLeft, SideRightFlipped, SideRightUp, SideRightDown,
    SideRight, SideLeftFlipped, SideLeftDown, SideLeftUp,
    Left, Right, Down, Up,
    RightBack, LeftBack, UpBack, DownBack,
    StraightBack, Straight, StraightFlipped, StraightBackFlipped,
    StraightBackRight, StraightLeft, StraightBackLeft, StraightRight
};
const uint8_t zNegativeTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    SideRight, SideLeftFlipped, SideLeftDown, SideLeftUp,
    SideLeft, SideRightFlipped, SideRightUp, SideRightDown,
    Right, Left, Up, Down,
    LeftBack, RightBack, DownBack, UpBack,
    Straight, StraightBack, StraightBackFlipped, StraightFlipped,
    StraightRight, StraightBackLeft, StraightLeft, StraightBackRight
};


OrientationTypes GetNewOrientation(int16_t x, int16_t y, int16_t z, uint16_t threshold, OrientationTypes orientation)
{
  if (y > threshold)
  {
    orientation = yPositiveTransitionState[orientation];
  }
  else if (y < -threshold)
  {
    orientation = yNegativeTransitionState[orientation];
  }
  else if (z > threshold)
  {
    orientation = zPositiveTransitionState[orientation];
  }
  else if (z < -threshold)
  {
    orientation = zNegativeTransitionState[orientation];
  }
  else if (x > threshold)
  {
    orientation = xPositiveTransitionState[orientation];
  }
  else if (x < -threshold)
  {
    orientation = xNegativeTransitionState[orientation];
  }

  return orientation;
}

