#pragma once 

#include <algorithm>
#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cpp-GEngine/StateManager.h>
#include <cpp-GEngine/GameState.h>
#include <cpp-GEngine/GEngine.h>
#include <cpp-GEngine/Window.h>
#include <cpp-GEngine/Timing.h>

#include "PlayState.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// Some helpful constants for semi-fixed timestep
const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

int main(int argc, char** argv) {
	// Initialize the game engine
	GEngine::init();

	// Create the window
	GEngine::Window window;
	window.create("demo-platformer", SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// Set black background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Initialize the InputManager
	GEngine::InputManager inputManager;

	// Initialize the StateManager
    GEngine::StateManager stateManager;
	stateManager.changeState(new PlayState(stateManager, window, inputManager, 1));

	// Used to cap the FPS
	GEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(200.0f);
	float fps;

	// Start our previousTicks variable
	float previousTicks = SDL_GetTicks();

	// Main loop
	while (stateManager.running()) {
		fpsLimiter.beginFrame();

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
			stateManager.updateInputManager();
			stateManager.processEvents();
			stateManager.update(deltaTime);

			// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
			totalDeltaTime -= deltaTime;
			// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
			i++;
		}
		
		stateManager.updateCamera();
		stateManager.draw();

		// End the frame, limit the FPS, and get the current FPS.
		fps = fpsLimiter.endFrame();
		std::cout << fps << std::endl;
	}
	// Leaving the scope of 'state_machine' will cleanup the engine

    return 0;
}