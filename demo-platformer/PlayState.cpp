#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cpp-GEngine/StateMachine.h>
#include <cpp-GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(StateMachine& stateMachine, Window& window, InputManager& inputManager) : GameState(stateMachine, window, inputManager) {
	std::cout << ": GameState::PlayState initialized.." << std::endl;
}


PlayState::~PlayState() {
}

void PlayState::init() {
	// Set up the shaders
	initShaders();

	// Initialize our sprite batch
	_spriteBatch.init();

	// Set up the camera
	//_camera.init(_screenWidth, _screenHeight);

	initLevel();
}

void PlayState::initShaders() {
	// Compile the color shader.
	_textureProgram.compileShaders("../assets/shaders/textureShading.vert", "../assets/shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void PlayState::initLevel() {

}

void PlayState::processEvents() {
	//std::cout << ">> PlayState processEvents" << std::endl;
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
	//std::cout << ">> PlayState update" << std::endl;
	if (_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		_stateMachine.quit();
	}

	if (_inputManager.isKeyPressed(SDLK_RETURN)) {
		// Restart the level
		_stateMachine.changeState(new PlayState(_stateMachine, _window, _inputManager));
	}

}

void PlayState::draw() {
	//std::cout << ">> PlayState draw" << std::endl;
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable shaders
	_textureProgram.use();

	// Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	//glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	//GLint pUniform = _textureProgram.getUniformLocation("P");
	//glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Begin drawing
	_spriteBatch.begin();

	// Draw everything...
	// Draw everything...
	// Draw everything...

	// End sprite batch creation
	_spriteBatch.end();

	// Render to the screen
	_spriteBatch.renderBatch();

	// Disable shaders
	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}