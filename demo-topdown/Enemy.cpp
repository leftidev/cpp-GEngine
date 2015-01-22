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
	
	glm::vec4 aabbRect;
	aabbRect.x = m_position.x + 16.0f;
	aabbRect.y = m_position.y + 16.0f;
	aabbRect.z = width - 32.0f;
	aabbRect.w = height - 32.0f;
	spriteBatch.draw(aabbRect, uvRect, m_debugTextureID, 1.0f, m_color);
	
}

void Enemy::update(const std::vector<std::string>& levelData, Player* player, float deltaTime) {
	// Get the direction vector towards the player
	m_direction = glm::normalize(player->getPosition() - m_position);
	m_position += m_direction * m_speed * deltaTime;

	// Do collision
	collideWithLevel(levelData);
}

// Circular collision
bool Enemy::collideWithEntity(Entity* entity) {

	// Minimum distance before there is a collision
	const float MIN_DISTANCE = ENEMY_RADIUS * 2.0f;

	// Center position of this agent
	glm::vec2 centerPosA = m_position + glm::vec2(ENEMY_RADIUS);
	// Center position of the parameter agent
	glm::vec2 centerPosB = entity->getPosition() + glm::vec2(ENEMY_RADIUS);

	// Distance vector between the two agents
	glm::vec2 distVec = centerPosA - centerPosB;

	// Length of the distance vector
	float distance = glm::length(distVec);

	// Depth of the collision
	float collisionDepth = MIN_DISTANCE - distance;

	// If collision depth > 0 then we did collide
	if (collisionDepth > 0) {

		// Get the direction times the collision depth so we can push them away from each other
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		// Push them in opposite directions
		m_position += collisionDepthVec / 2.0f;
		entity->m_position -= collisionDepthVec / 2.0f;

		return true;
	}
	return false;
}