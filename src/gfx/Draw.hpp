/* Draw.hpp should take care of drawing with spritesheets.
 * I'll be reusing some of the original Zordzman source code
 * for this. - JavaCakess/vividMario52
 */

#pragma once

#include <cassert>
#include <stdio.h>
#include <SDL_opengl.h>
#include "Texture.hpp" // The original Zordzman used a custom SpriteSheet class.
                       // We'll probably enhance Texture.hpp and use that as
                       // a spritesheet.

// TODO: Add more shit

class Drawer {

public:
    static void draw(Texture const &spritesheet, int tx, int ty, float x,
                     float y, float w, float h);
    static void rect(float x, float y, float w, float h, bool filled = true);
    static void line(float x1, float y1, float x2, float y2);
};