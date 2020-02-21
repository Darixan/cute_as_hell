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
typedef double Vec[2];

class Player {
public:
    //Public Properties
    Vec pos;
    Vec vel; 
    bool isRunning;
    bool isRolling;
    bool isShooting;
    bool wasHit;
    

    //Public methods
    

    //Accessor functions
    int getHp();

    //Mutator functions
    void setHp(int newHp);

private:
    //Private Properties
    int hp;
};

extern void drawPlayer();
#endif
