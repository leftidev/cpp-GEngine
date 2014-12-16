#include <SDL/SDL.h>

#include <iostream>

#include "Player.h"


Player::Player() {
}

Player::~Player() {
}

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
    textureID = GEngine::ResourceManager::getTexture("../assets/textures/gizmo_52x52.png").id;

    width = 32.0f;
    height = 32.0f;

    _speed.x = 0.0f;
    _speed.y = 0.0f;

    _position = pos;
    _inputManager = inputManager;
    _camera = camera;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;
}

void Player::update(float deltaTime) {
    // Move left
	if (_inputManager->isKeyDown(SDLK_a) || _inputManager->isKeyDown(SDLK_LEFT)) {
		_speed.x = -MAX_VELOCITY;
    }
    // Move right
	else if (_inputManager->isKeyDown(SDLK_d) || _inputManager->isKeyDown(SDLK_RIGHT)) {
		_speed.x = MAX_VELOCITY;
    }
    else {
        _speed.x = 0.0f;
    }

    // Move on Y-axis
    _position.y += _speed.y * deltaTime;

    // Check collision on Y-axis
    collide(glm::fvec2(0.0f, _speed.y));

    // Move on X-axis
    _position.x += _speed.x * deltaTime;

    // Check collision on X-axis
    collide(glm::fvec2(_speed.x, 0.0f));
}

// Collisions
void Player::collide(glm::fvec2(speed)) {
}