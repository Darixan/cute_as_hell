//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    21 Feb 2020
//File:             adrianT.h
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains declaration of player-related classes



#ifndef _ADRIANT_H_
#define _ADRIANT_H_

//Defined types
typedef double Vec[3];


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

    int speed;  

    bool isRunning;
    bool isGrounded;
    bool isRolling;
    bool isShooting;
    bool isHit;
    
    //Public Methods
    void run(int input);
    void shoot();
    void roll();
    void isDamaged(int damage);
    void drawPlayer();
    void checkGrounded(Platform ground);

    //Constructors and Destructors
    Player(int initHp, Vec initPos);
    
    //Accessors
    int getHp();

    //Mutators
    void setHp(int newHp);

private:
    //Private Properties
    int hp;
};

#endif
