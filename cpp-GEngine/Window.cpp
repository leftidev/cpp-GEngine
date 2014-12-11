#include "Window.h"
#include "Errors.h"


Window::Window()
{
}

Window::~Window()
{
}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE)
	{
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}

	// Open a SDL window
	_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
	if (_sdlWindow == nullptr)
	{
		GEngine::fatalError("SDL Window could not be created!");
	}

	// Set up OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
	if (glContext == nullptr)
	{
		GEngine::fatalError("SDL_GL Context could not be created!");
	}

	// Set up GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		GEngine::fatalError("Could not initialize glew!");
	}

	// Check the OpenGL version
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

	// Set the background color to gray
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	// Set V-SYNC
	SDL_GL_SetSwapInterval(false);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer()
{
	// Swap the buffers.
	SDL_GL_SwapWindow(_sdlWindow);
}
