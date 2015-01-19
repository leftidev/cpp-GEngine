#include <iostream>

#include <SDL/SDL.h>

#include "Player.h"


Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
    m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/block_dude.png").id;

    width = 64.0f;
	height = 64.0f;

    m_speed.x = 12.0f;
    m_speed.y = 12.0f;

    m_position = pos;
    m_inputManager = inputManager;
    m_camera = camera;

    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}

void Player::update(float deltaTime) {
	if (m_inputManager->isKeyDown(SDLK_w)) {
		m_position.y += m_speed.y * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_s)) {
		m_position.y -= m_speed.y * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed.x * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed.x * deltaTime;
	}
}

// Collisions
void Player::collide(glm::fvec2(speed)) {
}