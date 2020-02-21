//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.cpp
//Author: Melanie Corral
//Date: 2020
//Last modified 21 Feb 2020
//
//Completed:
//Credit implementation
//To do list:
//Enemy AI attack/health
//Different Enemy types

#include <iostream>
#include "fonts.h"

void MC_Credits(int yres)
{
    	Rect r;
	unsigned int c = 0x00ff0000;
        r.bot = yres - 165;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, c, "Melanie Corral");
}
void Main_Menu(int yres)
{
    	Rect r;
	unsigned int c = 0x00ffff00;

	r.bot = yres - 165;
	r.left = 275;
	r.center = 0;
        ggprint16(&r, 16, c, "Menu Test");


}

