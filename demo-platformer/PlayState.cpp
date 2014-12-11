#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "PlayState.h"
#include <cpp-GEngine/Window.h>


PlayState::PlayState(StateMachine& stateMachine, Window& window) : GameState(stateMachine, window) {
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