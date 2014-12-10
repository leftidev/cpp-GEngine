#pragma once

#include "Entity.h"


class Tile : public Entity {
public:
    Tile();
    ~Tile();

    void init(int TextureID, glm::vec2 pos);
};

