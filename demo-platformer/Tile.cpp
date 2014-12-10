#include "Tile.h"


Tile::Tile() {
    // Empty
}

Tile::~Tile() {
    // Empty
}

void Tile::init(int TextureID, glm::fvec2 pos) {

    textureID = TextureID;

    width = 64.0f;
    height = 64.0f;

    _position = pos;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;
}