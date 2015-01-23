#include "Enemy.h"
#include <cpp-GEngine/ResourceManager.h>
#include <iostream>

const float ENEMY_RADIUS = 16.0f;

Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::init(GLuint textureID, glm::vec2 pos) {
	m_textureID = textureID;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	width = 64.0f;
	height = 64.0f;

	m_speed.x = 1.0f;
	m_speed.y = 1.0f;
}

void Enemy::draw(GEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;
	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);

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

void Enemy::update(const std::vector<std::string>& levelData, Player* player, float deltaTime) {
	// Get the direction vector towards the player
	m_direction = glm::normalize(player->getPosition() - m_position);
	m_position += m_direction * m_speed * deltaTime;

	// Do collision
	collideWithLevel(levelData);
}