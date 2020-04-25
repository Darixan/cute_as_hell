//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    24 Apr 2020
//File:             adrianT.h
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains declaration of player-related classes



#ifndef _ADRIANT_H_
#define _ADRIANT_H_

//Defined types
typedef double Vec[3];

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

    //Public Methods
    void drawBullet();
    void moveBullet(int speed);
    void drawBulletTracer();

    //Constructors
    Bullet(int bulletSize, Vec initPos, Vec initVel);
};

class Platform {
public:
    //Public Properties
    Vec pos;
    Vec vel;

    int size;
    int top;
    int center;
    int left;
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

    //int speed;  
    int size;
    int damageOutput;

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
 
    //Public Methods
    void run(int runVel);
    void jump(int jumpVel);
    void shoot(Bullet plBullet);
    void roll();
    void isDamaged(int damageInput);
    void drawPlayer();
    void checkPlatfColl(Platform ground);
    void applyGravity(float gravVel);
    void faceLeft(float faceDir);
    void faceRight(float faceDir);
    int applyPoison(int poisDam);

    //Constructors and Destructors
    Player(int initHp, int playerSize, Vec initPos);
    
    //Accessors
    int getHp();

    //Mutators
    void setHp(int newHp);

private:
    //Private Properties
    int hp;
};

extern void UpdatePlayerFacing(Player player, Bullet bullet);

#endif
