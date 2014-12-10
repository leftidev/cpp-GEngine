#include "ResourceManager.h"

namespace GEngine {

TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(std::string texturePath)
{
    return _textureCache.getTexture(texturePath);
}

}