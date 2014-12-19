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


PlayState::~PlayState() { }

void PlayState::init() {
	// Set up the shaders
	loadShaders();

	// Initialize our sprite batch
	m_spriteBatch.init();

	// Set up the camera
	m_camera.init(1024, 768);

	// Zoom out the camera by 2x
	const float CAMERA_SCALE = 1.0f;
	m_camera.setScale(CAMERA_SCALE);

	initLevel();
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
	// Initialize the player
	m_player = new Player();
	m_player->init(glm::fvec2(0, -(768 / 2)), &m_inputManager, &m_camera);

	// Add the enemies
	for (unsigned int i = 0; i < 10; i++) {
		m_enemies.push_back(new Enemy);
		m_enemies.back()->init(GEngine::ResourceManager::getTexture("../assets/textures/pacified_enemy_104x104.png").id, glm::fvec2(0.0f, 0.0f), glm::fvec2(420.0f - (i * 70), 768 / 2 - 70), EnemyType::X_MOVING);
	}
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

	m_player->update(m_enemies, deltaTime);

	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->update(m_player->projectiles, deltaTime);
	}
}

void PlayState::updateCamera() {
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

	const glm::vec2 enemyDimensions(48.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Begin drawing
	m_spriteBatch.begin();

	// Draw player
	m_player->draw(m_spriteBatch);

	// Draw the enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		if (m_camera.isBoxInView(m_enemies[i]->getPosition(), enemyDimensions)) {
			m_enemies[i]->draw(m_spriteBatch);
		}
	}

	for (unsigned int i = 0; i < m_player->projectiles.size(); i++) {
		m_player->projectiles[i]->draw(m_spriteBatch);
	}

	// End sprite batch creation
	m_spriteBatch.end();

	// Render to the screen
	m_spriteBatch.renderBatch();

	// Disable shaders
	m_shaderProgram.disable();

	// Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();
}