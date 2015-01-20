#include <iostream>
#include <algorithm>

#include <SDL/SDL.h>

#include "Player.h"


const float TILE_WIDTH = 32.0f;

Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
    m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/topdown_dude.png").id;

    width = 32.0f;
	height = 18.0f;

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

bool Player::collideWithLevel(const std::vector<std::string>& levelData) {

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

void Player::checkTilePosition(const std::vector<std::string>& levelData,
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

// AABB (Axis Aligned Bounding Box) collision
void Player::collideWithTile(glm::vec2 tilePos) {

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	// The minimum distance before a collision occurs
	const float MIN_DISTANCE_X = width / 2.0f + TILE_RADIUS;
	const float MIN_DISTANCE_Y = height / 2.0f + TILE_RADIUS;

	// Center position of the agent
	glm::vec2 centerAgentPos = m_position + glm::vec2(width / 2, height / 2);
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