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

const uint8_t xPositiveTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    StraightRight, StraightLeft, StraightFliped, Straight, SideRightUp, SideRightDown, SideLeftFliped, SideRight, Left, Right, Left, Right, Up, Up, Down, Down
};
const uint8_t xNegativeTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    StraightLeft, StraightRight, Straight, StraightFliped, SideLeftUp, SideLeftDown, SideLeft, SideRightFliped, Left, Right, Left, Right, Up, Up, Down, Down
};
const uint8_t yPositiveTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    Down, Up, Right, Left, Straight, StraightFliped, StraightLeft, StraightRight, SideLeftDown, SideRightDown, SideRightUp, SideLeftUp, SideLeft, SideRight, SideRightFliped, SideLeftFliped
};
const uint8_t yNegativeTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    Up, Down, Left, Right, StraightFliped, Straight, StraightRight, StraightLeft, SideLeftUp, SideRightUp, SideRightDown, SideLeftDown, SideRightFliped, SideLeftFliped, SideLeft, SideRight
};
const uint8_t zPositiveTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    SideLeft, SideRightFliped, SideRightUp, SideRightDown, Left, Right, Down, Up, Straight, Straight, StraightFliped, StraightFliped, StraightRight, StraightLeft, StraightLeft, StraightRight
};
const uint8_t zNegativeTransitionState[NUMBER_OF_ORIENTATIONS] =
{
    SideRight, SideLeftFliped, SideLeftDown, SideLeftUp, Right, Left, Up, Down, Straight, Straight, StraightFliped, StraightFliped, StraightRight, StraightLeft, StraightLeft, StraightRight
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
