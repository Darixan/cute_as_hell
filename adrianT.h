//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    24 Apr 2020
//File:             adrianT.h
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains declaration of player-related classes

#include "melanieC.h"

#ifndef _ADRIANT_H_
#define _ADRIANT_H_

//Defined types
typedef double Vec[3];

class Enemy;
class Bullet;
class Platform;
class Player;

class Bullet {
public:
    //Public Properties
    Vec pos;
    Vec vel;

    int size;
    int top;
    int left;
    int right;
    int bottom;

    bool inContact;

    //Public Methods
    void drawBullet(Player player);
    void moveBullet(int speed, float mov);
    void drawBulletTracer();
    void checkBulletColl(Bullet *bullet, Platform platf); 
    void checkBulletColl(Bullet *bullet, Enemy enem);

    //Constructors
    Bullet();
    Bullet(int bulletSize, Vec initPos, Vec initVel);
};

class Platform {
public:
    //Public Properties
    Vec pos;
    Vec vel;

    int size;
    int top;
    int left;
    int center;
    int right;
    int bottom;

    //Public Methods
    void drawPlatf(int length); 

    //Constructors
    Platform(int platfSize, Vec initPos, Vec initVel);

};

class Player {
public:
    //Public Properties
    Vec pos;
    Vec vel; 

    int hp;
    //int speed;  
    int size;
    int damageOutput;

    Bullet *ammo;

    int mag;
    int magMax;

    float faceDir;

    bool isRunning;
    bool isJumping;
    bool isGrounded;
    bool isRolling;
    bool isShooting;
    bool isHit;
    bool facingLeft;
    bool facingRight;
    bool isPoisoned;
    bool isReloading;
    bool isDead;
 
    //Public Methods
    void run(int runVel);
    void jump(int jumpVel);
    void shoot(Bullet *plBullet);
    void roll();
    void isDamaged(Enemy enem);
    void drawPlayer();
    void checkPlatfColl(Platform ground);
    void applyGravity(float gravVel);
    void faceLeft(float faceDir);
    void faceRight(float faceDir);
    void drawHealthBar(Vec pos);
    void drawAmmo(Player player);
    int applyPoison(int poisDam);

    //Constructors and Destructors
    Player(int initHp, int playerSize, Vec initPos);
    
    //Accessors
    int getHp();

    //Mutators
    void setHp(int newHp);

private:
    //Private Properties
    //int hp;
};

extern void UpdatePlayerFacing(Player *player, Bullet *bullet);
extern void CheckShot(Player *player);
extern void CheckReload(Player *player, bool pressed);
extern void UpdateBulletPhysics(Player *player);
extern void UpdateBulletRendering(Player *player);
extern void PrintGameOverScreen(Player *player, Vec pos);
#endif
