#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec3 tangent, glm::vec3 bitangent);
    Vertex();
    Vertex(const Vertex& vertex);
    Vertex(glm::vec3 position, glm::vec2 texCoord);
};
