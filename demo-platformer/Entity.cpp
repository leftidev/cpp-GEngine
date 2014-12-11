#include <algorithm>

#include "Entity.h"

#include <iostream>


Entity::Entity() {
}

Entity::~Entity() {
}

void Entity::draw(GEngine::SpriteBatch& _spriteBatch) {
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 destRect;
    destRect.x = _position.x;
    destRect.y = _position.y;
    destRect.z = width;
    destRect.w = height;

    _spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}

// AABB (Axis Aligned Bounding Box) collision
bool Entity::collideWithTile(int width, int height, Entity* entity) {
    // The minimum distance before a collision occurs
    const float MIN_DISTANCE_X = width / 2.0f + entity->width / 2.0f;
    const float MIN_DISTANCE_Y = height / 2.0f + entity->height / 2.0f;

    // Vector from center of agent to center of tile
    glm::vec2 distVec = (_position + glm::vec2(width / 2, height / 2)) - (entity->getPosition() + glm::vec2(entity->width / 2, entity->height / 2));

    // Get the depth of the collision
    float xDepth = MIN_DISTANCE_X - abs(distVec.x);
    float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

    // If both the depths are > 0, then we collided
    if (xDepth > 0 && yDepth > 0) {
        return true;
    }
    return false;
}