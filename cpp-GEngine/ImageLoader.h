#pragma once

#include <string>

#include "GLTexture.h"


namespace GEngine {
// Loads images into GLTextures
class ImageLoader {
public:
	static GLTexture loadPNG(std::string filePath);
};
}