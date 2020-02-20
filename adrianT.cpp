//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    19 Feb 2020
//File:             adrianT.cpp
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains a function which outputs my credits.
//
//Will contain function definitions of player-related methods.


#include "fonts.h"
#include "adrianT.h"

//-----------------------------------------------------------------------------
//Player Class method definitions

bool Player::isRolling(bool rollState)
{
    if (rollState)
        return true;
    else
        return false;
}

bool Player::isShooting(bool shootState)
{
    if (shootState)
        return true;
    else
        return false;
}

bool Player::isHit(bool hitState)
{
    if (hitState)
        return true;
    else
        return false;
}

int Player::getHp()
{
    return hp;
}

void Player::setHp(int newHp)
{
    hp = newHp;
}

void Player::setRolling(bool rollState)
{
    rolling = rollState;
}

void Player::setShooting(bool shootState)
{
    shooting = shootState;
}

void Player::setHit(bool hitState)
{
    hit = hitState;
}
//-----------------------------------------------------------------------------

void CreditsTitle(int yres)
{
    Rect TCredit;

    TCredit.bot = yres - 150;
    TCredit.left = 10;
    TCredit.center = 0;

    ggprint12(&TCredit, 16, 0x00ff0000, "Credits");
}
void AT_Credits(int yres)
{
    
    Rect name;
    //Rect credit;

    name.bot = yres - 180;
    name.left = 10;
    name.center = 0;

    //ggprint8b(&credit, 16, 0x00ff0000, "Character Design");
    ggprint8b(&name, 16, 0x00ff0000, "Adrian Jay Telan");
}
