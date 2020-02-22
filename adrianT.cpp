//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    21 Feb 2020
//File:             adrianT.cpp
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains a function which outputs my credits.
//
//Will contain function definitions of player-related methods.

#include <GL/glx.h>

#include "fonts.h"
#include "adrianT.h"
#include "rainforest.h"

//-----------------------------------------------------------------------------
//=================Class and Method definitions for 'adrianT.h'================

//Player Class method definitions
//Public Methods
void Player::run()
{
    if (isRolling || isHit)
        return;
}

void Player::shoot()
{
    if (isRolling || isHit)
        return;
}

void Player::roll()
{
    if (isRolling)
        return;
}

void Player::drawPlayer()
{
    int playerSize = 25;

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(pos[0] - playerSize, pos[1] - playerSize);
        glVertex2f(pos[0] - playerSize, pos[1] + playerSize);
        glVertex2f(pos[0] + playerSize, pos[1] + playerSize);
        glVertex2f(pos[0] + playerSize, pos[1] - playerSize);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

//Constructor
Player::Player(int initHp, Vec initPos)
{
    hp = initHp;
    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    //int speed = 0;

    vel[0] = 0;
    vel[1] = 0;
    vel[2] = 0;

    isRunning = false;
    isRolling = false;
    isShooting = false;
    isHit = false;
}

//Accessors
int Player::getHp()
{
    return hp;
}

//Mutators
void Player::setHp(int newHp)
{
    hp = newHp;
}

//-----------------------------------------------------------------------------

//Prints the word 'Credits' to the screen
void CreditsTitle(int yres)
{
    Rect TCredit;

    TCredit.bot = yres - 150;
    TCredit.left = 10;
    TCredit.center = 0;

    ggprint12(&TCredit, 16, 0x00ff0000, "Credits");
}

//Prints my name in the credits section
void AT_Credits(int yres)
{
    
    Rect name;
    //Rect credit;

    name.bot = yres - 180;
    name.left = 10;
    name.center = 0;

    //ggprint8b(&credit, 16, 0x00ff0000, "Character Design");
    ggprint8b(&name, 16, 0x00ff0000, "Adrian Jay Telan");
}

//Prints the current tasks I'm working on onto the screen
void PrintTasks(int yres)
{
    Rect taskList;
    Rect task[3];

    taskList.bot = yres - 320;
    taskList.left = 10;
    taskList.center = 0;

    ggprint12(&taskList, 16, 0x00ff0000, "TASKS");

    for (int i = 0; i < 3; i++) {
	    task[i].bot = yres - 350 - 20*i;
	    task[i].left = 10;
	    task[i].center = 0;

	    if (i == 0) {
	        ggprint8b(&task[i], 16, 0x00ff0000, 
                    "Analyze rainforest framework");
        }

        if (i == 1) {
            ggprint8b(&task[i], 16, 0x00ff0000, 
                    "Analyze walk framework");
        }

        if (i == 2) {
            ggprint8b(&task[i], 16, 0x00ff0000, "Get good at programming");
        }
    }
}

//Draws a square
void DrawSquare(int yres)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(0, yres - 120);
        glVertex2f(0, yres - 410);
        glVertex2f(200, yres - 410);
        glVertex2f(200, yres - 120);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
