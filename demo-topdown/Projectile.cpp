#include "Projectile.h"
#include "Enemy.h"


Projectile::Projectile(float speed, glm::fvec2 direction, glm::vec2 pos) {
	m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/bullet.png").id;
	m_speed = speed;
	m_direction = direction;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	startPosition = pos;

	width = 4.0f;
	height = 4.0f;
}

Projectile::~Projectile() { }

void Projectile::update(const std::vector<std::string>& levelData, float deltaTime) {
	m_position += m_direction * m_speed * deltaTime;

	// Do collision
	if (collideWithLevel(levelData)) {
		destroyed = true;
	}
}