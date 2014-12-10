#pragma once

#include <vector>

namespace GEngine {

class IOManager
{
public:
    static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
};

}