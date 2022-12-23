#pragma once

#include <string>

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
    
    Texture(const std::string path, const std::string directory, bool gamma = false); 
    Texture(const Texture& t);

    unsigned int TextureFromFile(bool gamma = false);
};

