#pragma once

#include "Entity.h"
#include "Player.h"


class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	void init(GLuint textureID, glm::vec2 pos);
	void draw(GEngine::SpriteBatch& spriteBatch);
	void update(const std::vector<std::string>& levelData, Player* player, float deltaTime);
	bool collideWithEntity(Entity* entity);
};