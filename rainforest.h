//Class:            Software Engineering 3350
//Author:           Gordon Griesel
//Modified by:      Adrian Jay Telan
//Last Modified:    20 Feb 2020
//File:             rainforest.h
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains class and function declarations from Professor Gordon Griesel's
//rainforest framework
//
//Main features to be pulled from the framework are its image drawing
//capabilities, game windows setup, the physics system, and the key and 
//mouse event checks.
//
//Not integrated into the program as of yet: need to transition the function
//definitions from the original source file into a new rainforest.cpp fiel
//

#ifndef _RAINFOREST_H_
#define _RAINFOREST_H_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

class Image {
public:
    //Public Properties
    int width, height;
    unsigned char *data;

    //Class Destructors and Constructors
    ~Image();
    Image(const char *fname);
};

class X11_wrapper {
private:
    //Private Properties
    Display *dpy;
    Window win;

public:
    //Class Destructors and Constructors
    ~X11_wrapper();
    X11_wrapper();

    //Public Methods
    void setTitle();
    void setupScreenRes(const int w, const int h);
    void reshapeWindow(int width, int height);
    void checkResize(XEvent *e);
    bool getXPending();
    XEvent getXNextEvent();
    void swapBuffers();
};

extern void initOpengl();
extern void checkMouse(XEvent *e);
extern int checkKeys(XEvent *e);
extern void init();
extern void physics();
extern void render();

#endif
