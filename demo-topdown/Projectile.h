#pragma once

#include "Entity.h"

class Enemy;

class Projectile : public Entity {
public:
	Projectile(float speed, glm::fvec2 direction, glm::vec2 pos);
	~Projectile();

	void update(const std::vector<std::string>& levelData, float deltaTime);

	bool destroyed = false;

	float m_speed;

	glm::vec2 startPosition;
};

