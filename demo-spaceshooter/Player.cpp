#include <iostream>

#include <SDL/SDL.h>

#include "Player.h"


Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
    m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/gizmo_52x52.png").id;

    width = 32.0f;
    height = 32.0f;

    m_speed.x = 0.0f;
    m_speed.y = 0.0f;

    m_position = pos;
    m_inputManager = inputManager;
    m_camera = camera;

    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}

void Player::update(std::vector<Enemy*> enemies, float deltaTime) {
	// Shoot projectile
	if (m_inputManager->isKeyPressed(SDLK_LCTRL)) {
		shootProjectile();
	}
    // Move left
	if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_LEFT)) {
		std::cout << m_position.x << std::endl;
		m_speed.x = -MAX_VELOCITY;
    }
    // Move right
	else if (m_inputManager->isKeyDown(SDLK_d) || m_inputManager->isKeyDown(SDLK_RIGHT)) {
		m_speed.x = MAX_VELOCITY;
    }
    else {
        m_speed.x = 0.0f;
    }

    // Move on Y-axis
    m_position.y += m_speed.y * deltaTime;

    // Check collision on Y-axis
	collide(glm::fvec2(0.0f, m_speed.y), enemies);

    // Move on X-axis
    m_position.x += m_speed.x * deltaTime;

    // Check collision on X-axis
	collide(glm::fvec2(m_speed.x, 0.0f), enemies);

	// Update projectiles
	for (unsigned int i = 0; i < projectiles.size(); i++) {
		projectiles[i]->update(enemies, deltaTime);

		// Projectile hits 
		if (projectiles[i]->destroyed) {
			projectiles.erase(projectiles.begin() + i);
		}
		if (projectiles[i]->getPosition().y < projectiles[i]->startPosition.y - PROJECTILE_REACH ||
			projectiles[i]->getPosition().y > projectiles[i]->startPosition.y + PROJECTILE_REACH) {
			projectiles.erase(projectiles.begin() + i);
		}
	}
}

// Collisions
void Player::collide(glm::fvec2(speed), std::vector<Enemy*> enemies) {
}

void Player::shootProjectile() {
	if (projectiles.size() < 5) {
		projectiles.push_back(new Projectile(glm::fvec2(0.0f, 10.0f), glm::vec2(m_position.x + 11, m_position.y - 36)));
	}
}