//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.h
//Author: Melanie Corral
//Date: 2020
//Last modified: 24 Feb 2020
//
//This is the header file for Enemy properties
//

#ifndef _MELANIEC_H
#define _MELANIEC_H

/*
typedef struct function {
	int up;
} name;
*/

typedef double Vec[3];

extern void function(void);

class Enemy {
    public:
	int hp;
	int damage;
	Vec pos;
	Vec vel;

	bool isAttacking;
	bool isHit;

	void drawEnemy(int);

};


#endif
