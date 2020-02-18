#include "fonts.h"

void CreditsPage(int yres)
{
    Rect credit;
    Rect name;
    
    name.bot = yres;
    name.left = 10;
    name.center = 0;

    //ggprint8b(&credit, 16, 0x00ff0000, "Character Design");
    ggprint8b(&name, 16, 0x00ff0000, "Adrian Jay Telan");
}
