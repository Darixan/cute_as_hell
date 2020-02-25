//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.cpp
//Author: Melanie Corral
//Date: 2020
//Last modified 24 Feb 2020
//
//Completed:
//Credit implementation
//To do list:
//Enemy AI attack/health
//Different Enemy types

#include <iostream>
#include "fonts.h"
#include "melanieC.h"
#include "rainforest.h"

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
        ggprint16(&r, 16, c, "Beta Test");
}
void Enemy::drawEnemy(int yres)
{
    glDisable(GL_TEXTURE_2D);
    glColor3ub(242, 83, 0);
    glBegin(GL_TRIANGLES);
    //glVertex2f(-10.0f, -10.0f);
    //glVertex2f(  0.0f, 20.0f);
    //glVertex2f( 10.0f, -10.0f);
    glVertex2f(150, yres - 350); //top vertex
    glVertex2f(120, yres - 410); //left vertex
    glVertex2f(180, yres - 410); //right vertex

    glEnd();
    glEnable(GL_TEXTURE_2D);



} 

