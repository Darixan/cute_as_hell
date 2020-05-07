//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.h
//Author: Melanie Corral
//Date: 2020
//Last modified: 5 May 2020
//
//This is the header file for Enemy properties
//

#ifndef _MELANIEC_H
#define _MELANIEC_H

#include "adrianT.h"
/*
typedef struct function {
	int up;
} name;
*/

typedef double Vec[3];

//extern void function(void);
void menu();

class Enemy {
    public:
	//int hp;
	int damage;
	int increment;
	int esize;

	Vec pos;
	Vec vel;

	bool isWalking;	
	bool isGrounded;
	bool isAttacking;
	bool isHit;
	bool isDead;
	bool seesPlayer;

	void movement(Platform ground);
	void meleeBehavior(Player player, Platform ground);
	void Direction();
	void drawEnemy();
	void CollisonGround(Platform ground);
	void PlatformMovement(Platform ground);
	void CheckBullet(Bullet *plBullet);
	void Attack(Player play);

	Enemy(int enemyHP, int enemySize, Vec enemyPos);
    private:
	int ehp;
};

class Bullets {
	public:
		Vec pos;
		Vec vel;
		float color[3];
		struct timespec time;
	public:
		Bullets() { }
};

class ShootingEnemy
{
	public:
		Vec vel;
		Vec pos;
		float inc;
		void BasicEnemy();
};

#endif
