#include "Tile.h"


Tile::Tile() {
    // Empty
}

Tile::~Tile() {
    // Empty
}

void Tile::init(int TextureID, glm::fvec2 pos) {
    m_textureID = TextureID;

    width = 64.0f;
    height = 64.0f;

    m_position = pos;

    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}