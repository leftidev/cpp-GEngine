#pragma once

#include <string>
#include <vector>

#include <cpp-GEngine/SpriteBatch.h>

#include "Tile.h"


const int TILE_WIDTH = 32;

class Level {
public:
    // Load the level
    Level(const std::string& fileName);
    ~Level();

    void draw();

    // Getters
    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }

    std::vector<Tile*> _tiles;

private:
    std::vector<std::string> _levelData;

    glm::vec2 _startPlayerPos;
};

