#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cpp-GEngine/StateManager.h>
#include <cpp-GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel) :
	GameState(stateMachine, window, inputManager, currentLevel),
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
	m_camera.init(1024, 768);

	// Zoom out the camera by 2x
	const float CAMERA_SCALE = 1.0f / 2.0f;
	m_camera.setScale(CAMERA_SCALE);

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
	if (m_currentLevel == 1) {
		_levels.push_back(new Level("../assets/levels/level01.txt"));
	}
	else if (m_currentLevel == 2) {
		_levels.push_back(new Level("../assets/levels/level02.txt"));
	}

	// Initialize the player
	_player = new Player();
	_player->init(_levels[_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera);
}

void PlayState::processEvents() {
	SDL_Event evnt;
	// Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_stateMachine.quit();
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(float(evnt.motion.x), float(evnt.motion.y));
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void PlayState::update(float deltaTime) {
	if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		m_stateMachine.quit();
	}
	if (m_inputManager.isKeyPressed(SDLK_RETURN)) {
		// Restart the level
		m_stateMachine.changeState(new PlayState(m_stateMachine, m_window, m_inputManager, m_currentLevel));
	}
	if (m_inputManager.isKeyPressed(SDLK_1)) {
		glClearColor(1.f, 0.5f, 0.5f, 1.f);
	}
	if (m_inputManager.isKeyPressed(SDLK_2)) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	}

	_player->update(_levels[_currentLevel]->_tiles, deltaTime);
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	m_camera.setPosition(_player->getPosition());
	m_camera.update();
}

void PlayState::draw() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable shaders
	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const glm::vec2 tileDimensions(64.0f);

	// Begin drawing
	_spriteBatch.begin();

	// Draw tiles with camera culling
	for (int i = 0; i < _levels[_currentLevel]->_tiles.size(); i++) {
		if (m_camera.isBoxInView(_levels[_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
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
	m_window.swapBuffer();
}