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

void Init_Graphics(void)
{
	ssd1306_Init();

	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_Fill(Black);

	ssd1306_UpdateScreen();
}

void Update_Graphics(void)
{
	ssd1306_UpdateScreen();
}

void Previous_Window()
{
	;
}

void Next_Window()
{
	;
}



