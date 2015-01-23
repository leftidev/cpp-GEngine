#include <algorithm>
#include <iostream>

#include "Entity.h"


const float TILE_WIDTH = 64.0f;

Entity::Entity() { }

Entity::~Entity() { }

void Entity::draw(GEngine::SpriteBatch& spriteBatch) {
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 destRect;
    destRect.x = m_position.x;
    destRect.y = m_position.y;
    destRect.z = width;
    destRect.w = height;

    spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);
}

// Circular collision
bool Entity::collideWithEntity(Entity* entity) {

	// Minimum distance before there is a collision
	const float MIN_DISTANCE = 16.0f * 2.0f;

	// Center position of this agent
	glm::vec2 centerPosA = m_position + glm::vec2(16.0f);
	// Center position of the parameter agent
	glm::vec2 centerPosB = entity->getPosition() + glm::vec2(16.0f);

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

bool Entity::collideWithLevel(const std::vector<std::string>& levelData) {

	std::vector<glm::vec2> collideTilePositions;

	// Check the four corners
	// First corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x,
		m_position.y);
	// Second Corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x + width,
		m_position.y);

	// Third Corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x,
		m_position.y + width);

	// Third Corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x + width,
		m_position.y + width);

	// Check if there was no collision
	if (collideTilePositions.size() == 0) {
		return false;
	}

	// Do the collision
	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

void Entity::checkTilePosition(const std::vector<std::string>& levelData,
	std::vector<glm::vec2>& collideTilePositions,
	float x,
	float y) {

	// Get the position of this corner in grid-space
	glm::vec2 gridPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	// If we are outside the world, just return
	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return;
	}

	// If this is not an air tile, we should collide with it
	if (levelData[gridPos.y][gridPos.x] != '.') {
		collideTilePositions.push_back(gridPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

// AABB (Axis Aligned Bounding Box) collision. TODO: Don't hardcode the bounding box!
void Entity::collideWithTile(glm::vec2 tilePos) {

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	// The minimum distance before a collision occurs
	const float MIN_DISTANCE_X = (width - 32.0f) / 2.0f + TILE_RADIUS;
	const float MIN_DISTANCE_Y = (height - 32.0f) / 2.0f + TILE_RADIUS;

	// Center position of the agent
	glm::vec2 centerAgentPos = glm::vec2(m_position.x + 16.0f, m_position.y + 16.0f) + glm::vec2((width - 32.0f) / 2, (height - 32.0f) / 2);
	// Vector from the agent to the tile
	glm::vec2 distVec = centerAgentPos - tilePos;

	// Get the depth of the collision
	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	// If both the depths are > 0, then we collided
	if (xDepth > 0 && yDepth > 0) {

		// Check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X collision depth is smaller so we push in X direction
			if (distVec.x < 0) {
				m_position.x -= xDepth;
			}
			else {
				m_position.x += xDepth;
			}
		}
		else {
			// Y collision depth is smaller so we push in X direction
			if (distVec.y < 0) {
				m_position.y -= yDepth;
			}
			else {
				m_position.y += yDepth;
			}
		}
	}
}

// AABB (Axis Aligned Bounding Box) collision
bool Entity::collideWithEnemy(int width, int height, Entity* entity) {
	// The minimum distance before a collision occurs
	const float MIN_DISTANCE_X = width / 2.0f + (entity->width / 2.0f) / 2.0f;
	const float MIN_DISTANCE_Y = height / 2.0f + (entity->height / 2.0f) / 2.0f;

	// Vector from center of agent to center of tile
	glm::vec2 distVec = (m_position + glm::vec2(width / 2, height / 2)) - ((entity->getPosition() + glm::fvec2(16.0f)) + glm::vec2((entity->width / 2.0f) / 2, (entity->height / 2.0f) / 2));

	// Get the depth of the collision
	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	// If both the depths are > 0, then we collided
	if (xDepth > 0 && yDepth > 0) {
		return true;
	}
	return false;
}