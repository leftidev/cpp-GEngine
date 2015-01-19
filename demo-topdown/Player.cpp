#include <iostream>

#include <SDL/SDL.h>

#include "Player.h"


Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
    m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/flea_spaceship.png").id;

    width = 48.0f;
    height = 41.0f;

    m_speed.x = 0.0f;
    m_speed.y = 0.0f;

    m_position = pos;
    m_inputManager = inputManager;
    m_camera = camera;

    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}

void Player::update(float deltaTime) {
    // Move left
	if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_LEFT)) {
		m_speed.x = -MAX_VELOCITY;
    }
    // Move right
	else if (m_inputManager->isKeyDown(SDLK_d) || m_inputManager->isKeyDown(SDLK_RIGHT)) {
		m_speed.x = MAX_VELOCITY;
    }
    else {
        m_speed.x = 0.0f;
    }

    // Move on Y-axis
    m_position.y += m_speed.y * deltaTime;

    // Check collision on Y-axis
	collide(glm::fvec2(0.0f, m_speed.y));

    // Move on X-axis
    m_position.x += m_speed.x * deltaTime;

    // Check collision on X-axis
	collide(glm::fvec2(m_speed.x, 0.0f));
}

// Collisions
void Player::collide(glm::fvec2(speed)) {
}