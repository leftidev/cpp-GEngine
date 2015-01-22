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

void Tile::draw(GEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;
	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);

	// Debug bounding box drawing
	/*
	glm::vec4 aabbRect;
	aabbRect.x = m_position.x;
	aabbRect.y = m_position.y;
	aabbRect.z = width;
	aabbRect.w = height;
	spriteBatch.draw(aabbRect, uvRect, m_debugTextureID, 1.0f, m_color);
	*/
}