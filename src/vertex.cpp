#include "vertex.hpp"

Vertex::Vertex() {}

Vertex::Vertex(const Vertex& vertex) {
    Position = vertex.Position;
    Normal = vertex.Normal;
    TexCoords = vertex.TexCoords;
    Tangent = vertex.Tangent;
    Bitangent = vertex.Bitangent;
}

Vertex::Vertex(glm::vec3 position, glm::vec2 texCoord) {
    Position = position;
    TexCoords = texCoord;
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec3 tangent, glm::vec3 bitangent) : Position(position), Normal(normal), TexCoords(texCoord), Tangent(tangent), Bitangent(tangent) {}
