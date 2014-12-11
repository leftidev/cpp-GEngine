#include "SpriteBatch.h"

#include <algorithm>
#include <iostream>

namespace GEngine {
SpriteBatch::SpriteBatch() : 
	_vbo(0),
	_vao(0) {
}


SpriteBatch::~SpriteBatch() {
}

void SpriteBatch::init() {
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
	_sortType = sortType;
	_renderBatches.clear();
	_glyphs.clear();
}

void SpriteBatch::end() {
	// Set up all pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());
	for (int i = 0; i < _glyphs.size(); i++) {
		_glyphPointers[i] = &_glyphs[i];
	}

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
	_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::renderBatch() {
	// Bind our VAO. This sets up the OpenGL state we need, including
	// the vertex attribute pointers and it binds the VBO
	glBindVertexArray(_vao);

	for (int i = 0; i < _renderBatches.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() {
	// This will store all the vertices that we need to upload
	std::vector<Vertex> vertices;
	// Resize the buffer to the exact size we need so we can treat it like an array
	vertices.resize(_glyphPointers.size() * 6);

	if (_glyphPointers.empty()) {
		return;
	}

	int offset = 0; // current offset
	int cv = 0;     // current vertex

	// Add the first batch
	_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;
	offset += 6;

	// Add all the rest of the glyphs
	for (int cg = 1; cg < _glyphPointers.size(); cg++) {
		// Check if this glyph can be part of the current batch
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
			// Make a new batch
			_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
		}
		else {
			// If its part of the current batch, just increase numVertices
			_renderBatches.back().numVertices += 6;
		}

		vertices[cv++] = _glyphPointers[cg]->topLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->topRight;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		offset += 6;
	}

	//std::cout << "size of renderbatches: " << _renderBatches.size() << "\n";

	// Bind our VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Orphan the buffer (for speed)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	// Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::createVertexArray() {
	// Generate the VAO if it isn't already generated
	if (_vao == 0) {
		glGenVertexArrays(1, &_vao);
	}

	// Bind the VAO. All subsequent OpenGL calls will modify it's state
	glBindVertexArray(_vao);

	// Generate the VBO if it isn't already generated
	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// Tell OpenGL what attribute arrays we need.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// This is the position attribute pointer.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// This is the color attribute pointer.
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// This is the UV attribute pointer.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs()
{
	switch (_sortType) {
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
}

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return (a->depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
	return (a->texture < b->texture);
}
}