#include <cstddef>

#include "Vertex.h"
#include "ResourceManager.h"
#include "Sprite.h"


namespace GEngine {

Sprite::Sprite() {
	_vboID = 0;
}


Sprite::~Sprite() {
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

// Initializes the sprite VBO. x, y, width, and height are
// in the normalized device coordinate space. so, [-1, 1].
void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	_texture = ResourceManager::getTexture(texturePath);

	// Generate the buffer if it hasn't already been generated
	if (_vboID == 0) {
		// Generate vertex buffer object ID
		glGenBuffers(1, &_vboID);
	}

	// This array will hold our vertex data
	// We need 6 vertices, and each vertex has 2 floats for X and Y
	Vertex vertexData[6];

	// First Triangle
	// Top right
	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1.0f, 1.0f);

	// Top left
	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0.0f, 1.0f);

	// Bottom left
	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 0.0f);

	// Second Triangle
	// Bottom left
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0.0f, 0.0f);

	// Bottom right
	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1.0f, 0.0f);

	// Top right
	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1.0f, 1.0f);

	for (int i = 0; i < 6; i++) {
		vertexData[i].setColor(255, 255, 255, 255);
	}

	// Bind the vertex buffer object (active buffer)
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	// Upload the buffer data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	// Unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// Draws the sprite to the screen
void Sprite::draw() {
	glBindTexture(GL_TEXTURE_2D, _texture.id);
	// Bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	// Tell OpenGL that we want to use the first attribute array
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// Draw the 6 vertices to the screen
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Disable the vertex attrib array
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}