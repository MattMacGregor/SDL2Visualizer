#include "Terrain.hpp"

#include "stb/stb_image.h"
#include <iostream>

// Constructor for our object
// Calls the initialization method
Terrain::Terrain(unsigned int xSegs, unsigned int zSegs, std::string fileName, std::string colorFileName) : t(colorFileName, "/home/mmac/3dVisualizer"), xSegments(xSegs), zSegments(zSegs) {
    t.type = "texture_diffuse";
    // Load up some image data
    heightMap = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
    float scale = 0.5f; // Note that this scales down the values to make
    // Create height data
    heightData = new int[xSegments*zSegments];
    // Set the height data equal to the grayscale value of the heightmap
    // Because the R,G,B will all be equal in a grayscale image, then
    // we just grab one of the color components.

     for(unsigned int z = 0; z < zSegments; ++z) {
        for(unsigned x = 0; x < xSegments; ++x) {
            heightData[x + z * xSegments] = (float)GetPixelR(z, x)/scale;
            //std::cout << (float)GetPixelR(z, x)/scale << std::endl;
        }
     }

     for(unsigned int z = 0; z < zSegments; ++z) {
        for(unsigned x = 0; x < xSegments; ++x) {
            float u = 1.0f - ((float)x/(float)xSegments);
            float v = 1.0f - ((float)z/(float)zSegments);

            vertices.push_back({ glm::vec3(x, heightData[x+z*xSegments], z), glm::vec2(u, v) } );
        }
   }

    // Figure out which indices make up each triangle
    // By writing out a few of the indices you can figure out
    // the pattern here. Note there is an offset.
    
    // TODO: (Inclass) Build triangle strip
    for(unsigned int z = 0; z < zSegments - 1; ++z) {
        for(unsigned int x = 0; x < xSegments - 1; ++x) {
            indices.push_back(x + (z*zSegments));
            indices.push_back(x + (z*zSegments) + xSegments);
            indices.push_back(x + (z*zSegments) + 1);

            indices.push_back(x + (z*zSegments) + 1);
            indices.push_back(x + (z*zSegments) + xSegments);
            indices.push_back(x + (z*zSegments) + xSegments + 1);
        }
    }

    m = new Mesh(vertices, indices, { t });
}

// Destructor
Terrain::~Terrain(){
    // Delete our allocatted higheithmap data
    
    if(heightMap != nullptr) {
        stbi_image_free(heightMap);
    }

    if(heightData!=nullptr){
        delete heightData;
    }

    if(m != nullptr) {
        delete m;
    }
}

void Terrain::Draw(Shader& shader) {
    shader.use();
    shader.setMat4("model", transform.getModelMatrix());
    m->Draw(shader);
}

Transform& Terrain::getTransform() {
    return transform;
}
