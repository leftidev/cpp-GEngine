// GEngine uses code from "Bengine". (The game engine created in Benjamin Arnold's youtube tutorial series "Advanced C++/Graphics Tutorials")

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GEngine.h"

#include <iostream>

namespace GEngine {

int init()
{
    // Initialize SDL.
    SDL_Init(SDL_INIT_EVERYTHING);

    // Tells SDL to use double-buffering.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return 0;
}

}