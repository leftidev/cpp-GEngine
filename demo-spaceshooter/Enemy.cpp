#include "Enemy.h"

#include <iostream>


Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::init(int TextureID, glm::fvec2 speed, glm::vec2 pos, EnemyType enemyType) {
	m_textureID = TextureID;

	m_speed = speed;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	type = enemyType;

	width = 59.0f;
	height = 47.0f;
}

void Enemy::draw(GEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;

	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color);
}

void Enemy::update(std::vector<Projectile*> projectiles, float deltaTime) {

	// Move on Y-axis
	m_position.y += m_speed.y * deltaTime;

	m_position.x += m_speed.x * deltaTime;
}

void Enemy::applyJump() {
	inAir = true;
	jumped = true;

	if (type == JUMPING || X_MOVINGJUMPING) {
		m_speed.y = JUMP_SPEED;
	}
	if (type == REVERSEJUMPING) {
		m_speed.y = -JUMP_SPEED;
	}
}

void Enemy::applyCollisions(glm::fvec2(speed), std::vector<Projectile*> projectiles) {
	// Collide with level tiles
	for (unsigned int i = 0; i < projectiles.size(); i++) {
		if (collideWithEntity((int)width, (int)height, projectiles[i])) {
			
		}
	}
}