/*
 * lookup_table.h
 *
 *  Created on: Sep 15, 2023
 *      Author: Uros Cvjetinovic
 */

#ifndef INC_MOTION_TRACKING_H_
#define INC_MOTION_TRACKING_H_

#define NUMBER_OF_ORIENTATIONS ((uint8_t) 16u)

typedef enum OrientationTypesEnumeration {
  Straight  = 0,
  StraightFliped = 1,
  StraightLeft = 2,
  StraightRight = 3,
  Up = 4,
  Down = 5,
  Left = 6,
  Right = 7,
  SideLeft = 8,
  SideRight = 9,
  SideLeftFliped = 10,
  SideRightFliped = 11,
  SideLeftUp = 12,
  SideRightUp = 13,
  SideLeftDown = 14,
  SideRightDown = 15,
  DirectionTypes_EnumCount = 16
} OrientationTypes;

extern const char* OrientationTypeNames[];

OrientationTypes GetNewOrientation(int16_t x, int16_t y, int16_t z, uint16_t threshold, OrientationTypes orientation);
char* GetOrientationName(OrientationTypes orientation);

#endif /* INC_MOTION_TRACKING_H_ */
