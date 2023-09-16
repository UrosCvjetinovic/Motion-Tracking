/*
 * lookup_table.h
 *
 *  Created on: Sep 15, 2023
 *      Author: Uros Cvjetinovic
 */

#ifndef INC_MOTION_TRACKING_H_
#define INC_MOTION_TRACKING_H_

#define NUMBER_OF_ORIENTATIONS ((uint8_t) 24u)

typedef enum OrientationTypesEnumeration {
  Straight  = 0,
  StraightFlipped = 1,
  StraightLeft = 2,
  StraightRight = 3,
  StraightBack  = 4,
  StraightBackFlipped = 5,
  StraightBackLeft = 6,
  StraightBackRight = 7,
  Up = 8,
  Down = 9,
  Left = 10,
  Right = 11,
  UpBack = 12,
  DownBack = 13,
  LeftBack = 14,
  RightBack = 15,
  SideLeft = 16,
  SideRight = 17,
  SideLeftFlipped = 18,
  SideRightFlipped = 19,
  SideLeftUp = 20,
  SideRightUp = 21,
  SideLeftDown = 22,
  SideRightDown = 23,
  DirectionTypes_EnumCount = 24
} OrientationTypes;

extern const char* OrientationTypeNames[];

OrientationTypes GetNewOrientation(int16_t x, int16_t y, int16_t z, uint16_t threshold, OrientationTypes orientation);

#endif /* INC_MOTION_TRACKING_H_ */
