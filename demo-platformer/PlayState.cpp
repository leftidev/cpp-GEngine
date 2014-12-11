#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cpp-GEngine/StateMachine.h>
#include <cpp-GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(GEngine::StateMachine& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager) :
	GameState(stateMachine, window, inputManager),
	_fps(0),
	_player(nullptr) {
	std::cout << ": GameState::PlayState initialized.." << std::endl;
}


PlayState::~PlayState() {
	// Delete the levels
	for (unsigned int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
}

void PlayState::init() {
	// Set up the shaders
	initShaders();

	// Initialize our sprite batch
	_spriteBatch.init();

	// Set up the camera
	_camera.init(_window.getScreenWidth(), _window.getScreenHeight());

	initLevel();
}

void PlayState::initShaders() {
	// Compile the color shader
	_textureProgram.compileShaders("../assets/shaders/textureShading.vert", "../assets/shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void PlayState::initLevel() {
	// Initialize level 1
	_levels.push_back(new Level("../assets/levels/level01.txt"));
	_currentLevel = 0;

	// Initialize the player
	_player = new Player();
	_player->init(_levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera);
}

void PlayState::processEvents() {
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

void PlayState::update(float deltaTime) {
	if (_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		_stateMachine.quit();
	}

	if (_inputManager.isKeyPressed(SDLK_RETURN)) {
		// Restart the level
		_stateMachine.changeState(new PlayState(_stateMachine, _window, _inputManager));
	}
	if (_inputManager.isKeyPressed(SDLK_1)) {
		// Restart the level
		glClearColor(1.f, 0.5f, 0.5f, 1.f);
	}
	if (_inputManager.isKeyPressed(SDLK_2)) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	}

	_player->update(_levels[_currentLevel]->_tiles, deltaTime);
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	_camera.setPosition(_player->getPosition());
	_camera.update();
}

void PlayState::draw() {
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
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const glm::vec2 tileDimensions(64.0f);

	// Begin drawing
	_spriteBatch.begin();

	// Draw tiles with camera culling
	for (int i = 0; i < _levels[_currentLevel]->_tiles.size(); i++) {
		if (_camera.isBoxInView(_levels[_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
			_levels[_currentLevel]->_tiles[i]->draw(_spriteBatch);
		}
	}

	// Draw player
	_player->draw(_spriteBatch);

	// End sprite batch creation
	_spriteBatch.end();

	// Render to the screen
	_spriteBatch.renderBatch();

	// Disable shaders
	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}