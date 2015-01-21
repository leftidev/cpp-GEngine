#pragma once

#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/Camera2D.h>

#include "Entity.h"


class Player : public Entity {
public:
    Player();
    ~Player();

	void init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera);
	void draw(GEngine::SpriteBatch& spriteBatch);
	void update(const std::vector<std::string>& levelData, float deltaTime);

private:
	const float MAX_VELOCITY = 10.0f;
	const int PROJECTILE_REACH = 1000;

	GLuint m_textureID2;

	GEngine::InputManager* m_inputManager;
	GEngine::Camera2D* m_camera;
};

