#include "Projectile.h"
#include "Enemy.h"


Projectile::Projectile(glm::fvec2 speed, glm::vec2 pos) {
	m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/lazer.png").id;
	m_speed = speed;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	startPosition = pos;

	width = 4.0f;
	height = 32.0f;
}

Projectile::~Projectile() { }

void Projectile::update(std::vector<Enemy*> enemies, float deltaTime) {
	// Move on Y-axis
	m_position.y += m_speed.y * deltaTime;

	// Check collisions
	applyCollisions(enemies);
}

void Projectile::applyCollisions(std::vector<Enemy*> enemies) {
	// Collide with enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (collideWithEntity((int)width, (int)height, enemies[i])) {
			destroyed = true;
			enemies[i]->destroyed = true;
		}
	}
}