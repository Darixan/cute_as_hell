//Class:            Software Engineering 3350
//Author:           Gordon Griesel
//Modified by:      Adrian Jay Telan
//Last Modified:    20 Feb 2020
//File:             walk.h
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains class and function declarations from Professor Gordon Griesel's
//walk framework
//
//Main features to be pulled from the framework are its sprite animation
//capabilities and timing system
//
//Not yet integrated into the program: need to transition the function
//definitions from the original source file into a new walk.cpp file
//

#ifndef _WALK_H_
#define _WALK_H_

class Timers {
public:
    //Public Properties
    double physicsRate;
    double oobillion;
    struct timespec timeStart, timeEnd, timeCurrent;
    struct timespec walkTime;
    
    //Class Destructors and Constructors
    Timers();
    
    //Public Methods
    double timeDiff(struct timespec *start, struct timespec *end);
    void timeCopy(struct timespec *dest, struct timespec *source);
    void recordTime(struct timespec *t)
};

#endif
