/*
 * graphics.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Uros Cvjetinovic
 */

#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_

#include "lookup_table.h"
#include "main.h"

void InitGraphics(void);
void UpdateGraphics(void);
void SetScreen(ScreenTypes screen);
void PreviousScreen(void);
void NextScreen(void);

void DisplayConnectionStatus(void);
void DisplayOrientation(void);

#endif /* INC_GRAPHICS_H_ */
