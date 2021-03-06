#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cpp-GEngine/StateManager.h>
#include <cpp-GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel) :
	GameState(stateMachine, window, inputManager, currentLevel) {
	std::cout << ": GameState::PlayState initialized.." << std::endl;
}


PlayState::~PlayState() {
	// Delete the levels
	for (unsigned int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
}

void PlayState::init() {
	// Set up the shaders
	loadShaders();

	// Initialize our sprite batch
	m_spriteBatch.init();

	// Set up the camera
	m_camera.init(1024, 768);

	// Zoom out the camera by 2x
	const float CAMERA_SCALE = 1.0f / 2.0f;
	m_camera.setScale(CAMERA_SCALE);

	initLevel();

	// Clear color to blue
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
}

void PlayState::loadShaders() {
	// Create and compile the shaders
	m_shaders.push_back(GEngine::Shader("../assets/shaders/textureShading.vert", GL_VERTEX_SHADER));
	m_shaders.push_back(GEngine::Shader("../assets/shaders/textureShading.frag", GL_FRAGMENT_SHADER));
	// Add attributes to shaders
	m_shaderProgram.addAttribute("vertexPosition");
	m_shaderProgram.addAttribute("vertexColor");
	m_shaderProgram.addAttribute("vertexUV");
	// Link the shaders
	m_shaderProgram.linkShaders(m_shaders);
}

void PlayState::initLevel() {
	// Initialize level 1
	if (m_currentLevel == 0) {
		m_levels.push_back(new Level("../assets/levels/level01.txt"));
	}

	// Initialize the player
	m_player = new Player();
	m_player->init(m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera);
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

	m_player->update(m_levels[m_currentLevel]->_tiles, deltaTime);
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	m_camera.setPosition(m_player->getPosition());
	m_camera.update();
}

void PlayState::draw() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable shaders
	m_shaderProgram.enable();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_shaderProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_shaderProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const glm::vec2 tileDimensions(64.0f);

	// Begin drawing
	m_spriteBatch.begin();

	// Draw tiles with camera culling
	for (int i = 0; i < m_levels[m_currentLevel]->_tiles.size(); i++) {
		if (m_camera.isBoxInView(m_levels[m_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
			m_levels[m_currentLevel]->_tiles[i]->draw(m_spriteBatch);
		}
	}

	// Draw player
	m_player->draw(m_spriteBatch);

	// End sprite batch creation
	m_spriteBatch.end();

	// Render to the screen
	m_spriteBatch.renderBatch();

	// Disable shaders
	m_shaderProgram.disable();

	// Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();
}