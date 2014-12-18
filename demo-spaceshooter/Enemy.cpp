#include "Enemy.h"

#include <iostream>


Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::init(int TextureID, glm::fvec2 speed, glm::vec2 pos, EnemyType enemyType) {
	m_textureID = TextureID;
	textureID2 = GEngine::ResourceManager::getTexture("../assets/textures/pacified_enemy_104x104.png").id;

	m_speed = speed;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	type = enemyType;

	width = 48.0f;
	height = 48.0f;
}

void Enemy::draw(GEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;

	if (bubbled) {
		m_textureID = textureID2;
		destRect.x = m_position.x - 13.0f;
		destRect.y = m_position.y - 13.0f;;
		width = 104;
		height = 104;
	}

	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color);
}

void Enemy::update(std::vector<Projectile*> projectiles, float deltaTime) {
	// If bubbled, freeze the enemy
	if (!bubbled) {
		if (type == JUMPING || type == REVERSEJUMPING || type == X_MOVINGJUMPING) {
			if (!jumped && !inAir) {
				// Normal jumping
				applyJump();
			}

			// Player is in air, apply gravity
			if (inAir) {
				jumped = true;
				if (type == JUMPING || type == X_MOVINGJUMPING) {
					m_speed.y -= gravityAcceleration * deltaTime;
				}
				if (type == REVERSEJUMPING) {
					m_speed.y -= -gravityAcceleration * deltaTime;
				}
			}

			// Move on Y-axis
			m_position.y += m_speed.y * deltaTime;

			// Assume player is in air, this makes player fall off platform ledges
			inAir = true;

			// Check collisions on Y-axis
			applyCollisions(glm::fvec2(0.0f, m_speed.y), projectiles);
		}
		if (type == X_MOVING || type == X_MOVINGJUMPING) {
			// Move on X-axis
			m_position.x += m_speed.x * deltaTime;

			// Check collisions on X-axis
			applyCollisions(glm::fvec2(m_speed.x, 0.0f), projectiles);
		}
	}
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