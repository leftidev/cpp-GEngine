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
	const float CAMERA_SCALE = 1.5f;
	m_camera.setScale(CAMERA_SCALE);

	initLevel();

	// Clear color to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	// Add the enemies
	const std::vector<glm::vec2>& enemyPositions = m_levels[m_currentLevel]->enemyStartPositions;
	const std::vector<GLuint>& enemyTextureIDs = m_levels[m_currentLevel]->enemyTextureIDs;
	for (unsigned int i = 0; i < enemyPositions.size(); i++) {
		m_enemies.push_back(new Enemy);
		m_enemies.back()->init(enemyTextureIDs[i], enemyPositions[i]);
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

	updateEntities(deltaTime);
	updateBullets(deltaTime);
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	m_camera.setPosition(m_player->getPosition());
	m_camera.update();
}

void PlayState::updateEntities(float deltaTime) {
	m_player->update(m_levels[m_currentLevel]->getLevelData(), m_projectiles, deltaTime);

	// Update all enemies
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_player, deltaTime);
	}

	// Update Zombie collisions
	for (int i = 0; i < m_enemies.size(); i++) {
		// Collide with other zombies
		for (int j = i + 1; j < m_enemies.size(); j++) {
			m_enemies[i]->collideWithEntity(m_enemies[j]);
		}

		// Collide with player
		if (m_enemies[i]->collideWithEntity(m_player)) {
			std::cout << "Enemy hits player!" << std::endl;
		}

		// Collide with player
		if (m_enemies[i]->collideWithEntity(m_player)) {
			std::cout << "Enemy hits player!" << std::endl;
		}

		// Projectile hits 
		if (m_enemies[i]->destroyed) {
			m_enemies.erase(m_enemies.begin() + i);
		}
	}
}

void PlayState::updateBullets(float deltaTime) {
	// Update projectiles
	for (unsigned int i = 0; i < m_projectiles.size(); i++) {
		m_projectiles[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime);

		// Update Zombie collisions
		for (unsigned int j = 0; j < m_enemies.size(); j++) {
			// Collide with player
			if (m_projectiles[i].collideWithEnemy(m_projectiles[i].width, m_projectiles[i].height, m_enemies[j])) {
				m_projectiles[i].destroyed = true;
				m_enemies[j]->destroyed = true;
			}
		}

		// Projectile hits 
		if (m_projectiles[i].destroyed) {
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Begin drawing
	m_spriteBatch.begin(GEngine::GlyphSortType::FRONT_TO_BACK);

	// Draw tiles with camera culling
	for (int i = 0; i < m_levels[m_currentLevel]->_tiles.size(); i++) {
		if (m_camera.isBoxInView(m_levels[m_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
			m_levels[m_currentLevel]->_tiles[i]->draw(m_spriteBatch);
		}
	}

	// Draw player
	m_player->draw(m_spriteBatch);

	// Draw the zombies
	for (int i = 0; i < m_enemies.size(); i++) {
		if (m_camera.isBoxInView(m_enemies[i]->getPosition(), tileDimensions)) {
			m_enemies[i]->draw(m_spriteBatch);
		}
	}

	for (unsigned int i = 0; i < m_projectiles.size(); i++) {
		m_projectiles[i].draw(m_spriteBatch);
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