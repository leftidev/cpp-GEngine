#pragma once

#include "Entity.h"

class Enemy;

class Projectile : public Entity {
public:
	Projectile(glm::fvec2 speed, glm::vec2 pos);
	~Projectile();

	void update(std::vector<Enemy*> enemies, float deltaTime);
	void applyCollisions(std::vector<Enemy*> enemies);

	bool destroyed = false;

	glm::vec2 startPosition;
};

