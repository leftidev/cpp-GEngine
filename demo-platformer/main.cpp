#pragma once 

#include <algorithm>
#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cpp-GEngine/StateMachine.h>
#include <cpp-GEngine/GameState.h>

#include "PlayState.h"

int main(int argc, char** argv) {
	/*
	// Initialize the game engine
	GEngine::init();

	// Create our window
	_window.create("ARPG Experiment?", _screenWidth, _screenHeight, 0);

	// Black background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	*/

	// Initialize SDL.
	SDL_Init(SDL_INIT_EVERYTHING);

	// Tells SDL to use double-buffering.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Open a SDL window.
	SDL_Window* _sdlWindow = SDL_CreateWindow("demo-platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

	// Set up OpenGL context.
	SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);

	// Set up glew.
	GLenum error = glewInit();

	// Check the OpenGL version.
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

	// Set the background color to gray.
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	// Set V-SYNC.
	SDL_GL_SetSwapInterval(false);

	// Enable alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Some helpful constants.
	const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
	const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
	const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
	const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime


	// Zoom out the camera by 2x
	const float CAMERA_SCALE = 1.0f / 2.0f;
	//_camera.setScale(CAMERA_SCALE);

	// Start our previousTicks variable
	float previousTicks = SDL_GetTicks();

    StateMachine stateMachine;
	stateMachine.changeState(new PlayState(stateMachine));

	// Main loop
	while (stateMachine.running()) {
		// Calculate the frameTime in milliseconds
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
		// Get the total delta time
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		int i = 0; // This counter makes sure we don't spiral to death!
		// Loop while we still have steps to process.
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			// Update all physics here and pass in deltaTime
			stateMachine.inputManagerUpdate();
			stateMachine.processEvents();
			stateMachine.update();

			// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
			totalDeltaTime -= deltaTime;
			// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
			i++;
		}
		stateMachine.draw();
	}
	// Leaving the scope of 'state_machine' will cleanup the engine.

    return 0;
}