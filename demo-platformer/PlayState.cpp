#include <iostream>

#include "PlayState.h"


PlayState::PlayState(StateMachine& stateMachine) : GameState(stateMachine) {
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
}