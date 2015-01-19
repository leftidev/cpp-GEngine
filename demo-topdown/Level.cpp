#include <fstream>
#include <iostream>
#include <algorithm>

#include <cpp-GEngine/GEngineErrors.h>
#include <cpp-GEngine/ResourceManager.h>

#include "Level.h"


Level::Level(const std::string& fileName) {
    std::ifstream file;
    file.open(fileName);

    // Error checking
    if (file.fail()) {
        GEngine::fatalError("Failed to open " + fileName);
    }

    // Throw away the first string in tmp
    std::string tmp;

    file >> tmp;

    std::getline(file, tmp); // Throw away the rest of the first line

    // Read the level data
    while (std::getline(file, tmp)) {
        _levelData.emplace_back(tmp);
    }    
    
    // The level is flipped, reverse the vector
    std::reverse(_levelData.begin(), _levelData.end());

    // Render all the tiles
    for (int y = 0; y < _levelData.size(); y++) {
        for (int x = 0; x < _levelData[y].size(); x++) {
            // Grab the tile
            char tile = _levelData[y][x];

            // Process the tile
            switch (tile) {
				case '#': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/textures/black_tile.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case '@':
					_levelData[y][x] = '.'; /// So we dont collide with a @
                    _startPlayerPos.x = x * TILE_WIDTH;
                    _startPlayerPos.y = y * TILE_WIDTH;
                    break;
                case '.':
                    break;
                default:
                    std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
                    break;
            }
        }
    }
}

Level::~Level() {
    // Delete the tiles
    for (int i = 0; i < _tiles.size(); i++) {
        delete _tiles[i];
    }
}
