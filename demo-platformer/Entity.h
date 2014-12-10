#pragma once

#include <glm/glm.hpp>

#include <cpp-GEngine/SpriteBatch.h>
#include <cpp-GEngine/ResourceManager.h>


class Entity
{
public:
    Entity();
    virtual ~Entity();

    void draw(GEngine::SpriteBatch& _spriteBatch);

    glm::fvec2 getPosition() const { return _position; }

    float width;
    float height;

protected:
    bool collideWithTile(int width, int height, Entity* entity);



    int textureID;

    glm::fvec2 _position;
    GEngine::ColorRGBA8 _color;
    glm::fvec2 _speed;
};