//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    20 Feb 2020
//File:             adrianT.h
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains declaration of player-related classes



#ifndef _ADRIANT_H_
#define _ADRIANT_H_

//Defined types
typedef double Vec[3];

class Player {
public:
    //Public Properties
    Vec pos;
    Vec vel; 

    int speed;  

    bool isRunning;
    bool isRolling;
    bool isShooting;
    bool isHit;
    
    //Public Methods
    void run();
    void shoot();
    void roll();
    void isDamaged(int damage);

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

extern void drawPlayer();
#endif
