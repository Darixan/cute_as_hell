//Melanie Corral
//Credit implementation started
//
#include <iostream>
#include "fonts.h"

void MC_Credits(int yres)
{
    	Rect r;
	unsigned int c = 0x00ff0000;
        r.bot = yres - 165;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, c, "Melanie Corral");
}
