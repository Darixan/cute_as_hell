//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    26 Feb 2020
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

//Method Prototypes

//===============Class and Method definitions from 'adrianT.h'=================
//-----------------------------------------------------------------------------
//Bullet Class method definitions

//Public Methods
void Bullet::drawBullet() 
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(pos[0] - size, pos[1] + size);
        glVertex2f(pos[0] + size, pos[1] + size);
        glVertex2f(pos[0] + size, pos[1] - size);
        glVertex2f(pos[0] - size, pos[1] - size);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void Bullet::moveBullet()
{
    
}

void Bullet::bulletTracer()
{

}

//Constructors
Bullet::Bullet(int bulletSize, Vec initPos, Vec initVel)
{
    size = bulletSize;

    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    vel[0] = initVel[0];
    vel[1] = initVel[1];
    vel[2] = initVel[2];
}

//-----------------------------------------------------------------------------
//Platform Class method definitions

//Public Methods
void Platform::drawPlatf(int length)
{
    top = pos[1] + size;
    center = pos[0] + ((length - 1) * size);
    left = center - ((length) * size);
    right = center + ((length) * size);
    bottom = pos[1] - size;
    
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.2f);
    
    for (int i = 0; i < length; i++) {
        pos[0] += 2 * size;
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] - size);
            glVertex2f(pos[0] - size, pos[1] - size);
        glEnd();
    }
    
   glEnable(GL_TEXTURE_2D);
}

//Constructors
Platform::Platform(int platfSize, Vec initPos, Vec initVel)
{
    size = platfSize;
     
    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    vel[0] = initVel[0];
    vel[1] = initVel[1];
    vel[2] = initVel[2];
}

//Accessors

//Mutators

//-----------------------------------------------------------------------------
//Player Class method definitions

//Public Methods
void Player::run(int runVel)
{
    vel[0] = (float)runVel;
    faceRight();
    faceLeft();
    if (isRolling || isHit)
        return;
    else {
        pos[0] += vel[0];
        return;
    }
}

void Player::jump() 
{

}

void Player::shoot(Bullet plBullet)
{
    if (isRolling || isHit)
        return;
}

void Player::roll()
{
    if (isRolling)
        return;
}

void Player::isDamaged(int damageInput)
{
    hp -= damageInput;
    if (hp <= 0) {
        //Dead
    }
}

void Player::drawPlayer()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(pos[0] - size, pos[1] - size);
        glVertex2f(pos[0] - size, pos[1] + size);
        glVertex2f(pos[0] + size, pos[1] + size);
        glVertex2f(pos[0] + size, pos[1] - size);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void Player::checkGrounded(Platform ground)
{
    int plSoles = pos[1] - size;
    int plRight = pos[0] + size;
    int plLeft = pos[0] - size;
    //int plTop = pos[1] + size;


    if (plSoles <= ground.top && plRight >= ground.left && 
            plLeft <= ground.right) {
        isGrounded = true;
        vel[1] = 0;
        if ((!(plRight <= ground.left && plLeft >= ground.right) && 
                plSoles >= ground.bottom))
            pos[1] = ground.top + 25;
    } else {
        isGrounded = false;
        vel[1] -= 0.2;
        vel[1] *= 1.1;
        pos[1] += vel[1];
    }
}

void Player::faceLeft()
{
    if (vel[0] >= 0) {
        facingRight = false;
        facingLeft = true;
    } 
}

void Player::faceRight()
{
    if (vel[0] < 0) {
        facingLeft = false;
        facingRight = true;
    }
}

//Constructor
Player::Player(int initHp, int playerSize, Vec initPos)
{
    hp = initHp;
    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    //int speed = 0;
    size = playerSize;

    vel[0] = 0;
    vel[1] = 0;
    vel[2] = 0;

    isRunning = false;
    isJumping = false;
    isGrounded = false;
    isRolling = false;
    isShooting = false;
    isHit = false;
    facingLeft = false;
    facingRight = true;
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
//=============================================================================

//Uncategorized Methods
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

void UpdatePlayerFacing(Player player, Bullet bullet)
{
    if (player.facingRight) {
        bullet.pos[0] = player.pos[0] + player.size;
        bullet.pos[1] = player.pos[1];
    }

    if (player.facingLeft) {
        bullet.pos[0] = player.pos[0] - player.size;
        bullet.pos[1] = player.pos[1];
    }
}
