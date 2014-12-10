#pragma once

#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/Camera2D.h>

#include "Entity.h"
#include "Tile.h"


class Player : public Entity
{
public:
    Player();
    ~Player();

    void init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera);

    void update(std::vector<Tile*> tiles, float deltaTime);

    void jump();

    void collide(glm::fvec2(speed), std::vector<Tile*> tiles);

    float runningSpeed;

    bool inAir = true;

    bool jumped = false;

private:
    GEngine::InputManager* _inputManager;

    GEngine::Camera2D* _camera;
};

