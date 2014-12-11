#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GEngine.h"


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