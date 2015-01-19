#pragma once

#include <glm/glm.hpp>

#include <cpp-GEngine/SpriteBatch.h>
#include <cpp-GEngine/ResourceManager.h>


class Entity
{
public:
    Entity();
    virtual ~Entity();

    void draw(GEngine::SpriteBatch& spriteBatch);

    glm::fvec2 getPosition() const { return m_position; }

    float width;
    float height;

protected:
	bool collideWithEntity(int width, int height, Entity* entity);

    int m_textureID;

    glm::fvec2 m_position;
    GEngine::ColorRGBA8 m_color;
    glm::fvec2 m_speed;
};