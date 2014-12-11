#pragma once

#include <string>

#include "TextureCache.h"


namespace GEngine {
// This is a way to access all resources, such as models or textures
class ResourceManager {
public:
	static GLTexture getTexture(std::string texturePath);

private:
	static TextureCache _textureCache;
};
}