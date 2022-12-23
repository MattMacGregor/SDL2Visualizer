#pragma once

#include <glm/glm.hpp>

#include "mesh.h"
#include "transform.hpp"
#include "vertex.hpp"

class Square {
    glm::vec3 normal = {0.0f, 0.0f, -1.0f};
    glm::vec3 tangent = {0.0f, 1.0f, 0.0f};
    glm::vec3 bitangent = {1.0f, 0.0f, 0.0f};

    const std::vector<glm::vec3> verts = {
        glm::vec3(0.5f, 0.5f, -1.0f),
        glm::vec3(-0.5f, 0.5f, -1.0f),
        glm::vec3(-0.5f, -0.5f, -1.0f),
        glm::vec3(0.5f, -0.5f, -1.0f)
    };
    
    const std::vector<glm::vec2> texCoords = {
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
    };
    
    const std::vector<Vertex> vertices = {
        {verts[0], normal, texCoords[0], tangent, bitangent},
        {verts[1], normal, texCoords[1], tangent, bitangent},
        {verts[2], normal, texCoords[2], tangent, bitangent},
        {verts[3], normal, texCoords[3], tangent, bitangent}
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };
    
    Texture tex;
    Mesh m;
	Transform transform;

    public:
        Square(std::string fileName = "box.jpg") : tex(fileName, "/home/mmac/3dVisualizer"), m(vertices, indices, {tex}) {}

        Square(Texture& texture) : tex(texture), m(vertices, indices, {tex})  {}

        Mesh getMesh() {
            return m;
        }

        void Draw(Shader& shader) {
            shader.use();
			shader.setMat4("model", transform.getModelMatrix());
            m.Draw(shader);
        }

        Transform& getTransform() {
            return transform;
        }
};
