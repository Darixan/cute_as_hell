//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.cpp
//Author: Melanie Corral
//Date: 2020
//Last modified 1 March 2020
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
void Enemy::drawEnemy()
{
    glDisable(GL_TEXTURE_2D);
    glColor3ub(242, 83, 0);
    glBegin(GL_TRIANGLES);
    //glVertex2f(-10.0f, -10.0f);
    //glVertex2f(  0.0f, 20.0f);
    //glVertex2f( 10.0f, -10.0f);
    glVertex2f(pos[0], pos[1] + esize); //top vertex
    glVertex2f(pos[0] - esize, pos[1] - esize); //left vertex
    glVertex2f(pos[0] + esize, pos[1] - esize); //right vertex

    glEnd();
    glEnable(GL_TEXTURE_2D);



}
Enemy::Enemy(int enemyHP, int enemySize, Vec enemyPos)
{
	ehp = enemyHP;
	pos[0] = enemyPos[0];
	pos[1] = enemyPos[1];
	pos[2] = enemyPos[2];

	esize = enemySize;
	
	vel[0] = 0.5;
	vel[1] = 0;
	vel[2] = 0;

	isGrounded = false;
	
	/*isWalking = true;	
	if (isWalking == true) {
		enemyPos[0] += -.5; //left
		enemyPos[0] += .5; //right
		
	}*/
}

void Enemy::CollisonGround(Platform ground)
{

    int EnemyBottom = pos[1] - esize;
    int EnemyRight = pos[0] + esize;
    int EnemyLeft = pos[0] - esize;

    if (EnemyBottom <= ground.top && EnemyRight >= ground.left &&
            EnemyLeft <= ground.right) {
        isGrounded = true;
        vel[1] = 0;
    } else { 
	isGrounded = false;
        vel[1] = -6.80;
        pos[1] += vel[1];
    }

}

void Enemy::movement(Platform ground)
{	
	pos[0] += vel[0];
	pos[1] += vel[1];

	if(((pos[0] + esize) < ground.left && vel[0] < 0.0) ||
		        (pos[0] >= ground.left && vel[0] > 0.0))
 		vel[0] = -vel[0] * 6.5;
	if(((pos[0] - esize) < ground.right && vel[1] < 0.0) || 
			(pos[1] >= ground.right && vel[1] > 0.0))
		vel[1] = -vel[1] *5; 
/*	if ((pos[0] < -140.0 && vel[0] < 0.0) ||
		(pos[0] >= esize + 140.0 &&
		vel[0] > 0.0)) {

		vel[0] = -vel[0] * 5;

	}
	if ((pos[1] < 150.0 && vel[1] < 0.0) ||
		(pos[1] >= esize && vel[1] > 0.0)) {
		
		vel[1] = -vel[1] * 5;

	}
*/
}
