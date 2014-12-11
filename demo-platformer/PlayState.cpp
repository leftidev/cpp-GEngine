#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "PlayState.h"
#include <cpp-GEngine/StateMachine.h>
#include <cpp-GEngine/Window.h>


PlayState::PlayState(StateMachine& stateMachine, Window& window, InputManager& inputManager) : GameState(stateMachine, window, inputManager) {
}


PlayState::~PlayState()
{
}

void PlayState::init() {
	// Set up the shaders
	initShaders();

	// Initialize our sprite batch
	//_mainSpriteBatch.init();

	// Set up the camera
	//_camera.init(_screenWidth, _screenHeight);

	initLevel();
}

void PlayState::initShaders() {
	/*
	// Compile the color shader.
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
	*/
}

void PlayState::initLevel() {

}

void PlayState::processEvents() {
	std::cout << ">> PlayState processEvents" << std::endl;
	SDL_Event evnt;
	// Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_stateMachine.quit();
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(float(evnt.motion.x), float(evnt.motion.y));
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void PlayState::update() {
	std::cout << ">> PlayState update" << std::endl;
}

void PlayState::draw() {
	std::cout << ">> PlayState draw" << std::endl;
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}