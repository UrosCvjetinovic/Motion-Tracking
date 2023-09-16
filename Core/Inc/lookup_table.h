/*
 * lookup_table.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Uros Cvjetinovic
 */

#ifndef INC_LOOKUP_TABLE_H_
#define INC_LOOKUP_TABLE_H_

#include "stdint.h"

#define IMAGE_PLANE_SIZE ((uint8_t) 50u)
#define NUMBER_OF_PLANE_ORIENTATIONS ((uint8_t) 16u)

extern const uint8_t *planeImgs[];
//50x50px

extern const uint8_t planeImgStraight[];
extern const uint8_t planeImgStraightFliped[];
extern const uint8_t planeImgStraightLeft[];
extern const uint8_t planeImgStraightRight[];
extern const uint8_t planeImgSideLeft[];
extern const uint8_t planeImgSideRight[];
extern const uint8_t planeImgSideLeftFliped[];
extern const uint8_t planeImgSideRightFliped[];
extern const uint8_t planeImgSideRightUp[];
extern const uint8_t planeImgSideRightDown[];
extern const uint8_t planeImgSideLeftUp[];
extern const uint8_t planeImgSideLeftDown[];
extern const uint8_t planeImgUp[];
extern const uint8_t planeImgDown[];
extern const uint8_t planeImgLeft[];
extern const uint8_t planeImgRight[];
extern const uint8_t planeImgDownRight[];
extern const uint8_t planeImgDownLeft[];
extern const uint8_t planeImgUpLeft[];
extern const uint8_t planeImgUpRight[];

#endif /* INC_LOOKUP_TABLE_H_ */
