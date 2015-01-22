#include <iostream>
#include <algorithm>

#include <SDL/SDL.h>

#include "Player.h"


const float TILE_WIDTH = 64.0f;

Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
    m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/soldier_torso_1h.png").id;
	m_textureID2 = GEngine::ResourceManager::getTexture("../assets/textures/soldier_legs_0004.png").id;

    width = 64.0f;
	height = 64.0f;

    m_speed.x = 8.0f;
    m_speed.y = 8.0f;

    m_position = pos;
    m_inputManager = inputManager;
    m_camera = camera;

    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}

void Player::draw(GEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;
	spriteBatch.draw(destRect, uvRect, m_textureID, 0.5f, m_color, m_direction);
	spriteBatch.draw(destRect, uvRect, m_textureID2, 0.0f, m_color, m_direction);

	// Debug bounding box drawing
	/*
	glm::vec4 aabbRect;
	aabbRect.x = m_position.x + 16.0f;
	aabbRect.y = m_position.y + 16.0f;
	aabbRect.z = width - 32.0f;
	aabbRect.w = height - 32.0f;
	spriteBatch.draw(aabbRect, uvRect, m_debugTextureID, 1.0f, m_color);
	*/
}

void Player::update(const std::vector<std::string>& levelData, float deltaTime) {
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

	glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = m_position + glm::vec2(width / 2, height / 2);

	m_direction = glm::normalize(mouseCoords - centerPosition);

	collideWithLevel(levelData);
}

