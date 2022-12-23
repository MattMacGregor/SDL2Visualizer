/** @file Terrain.hpp
 *  @brief Create a terrain
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "texture.hpp"

#include <vector>
#include <string>
#include "vertex.hpp"
#include "transform.hpp"
#include "mesh.h"

class Terrain {
public:
    // Takes in a Terrain and a filename for the heightmap.
    Terrain (unsigned int xSegs, unsigned int zSegs, std::string heightFileName, std::string colorFileName = "");
    // Destructor
    ~Terrain ();
    // Loads a heightmap based on a PPM image
    // This then sets the heights of the terrain.
    void LoadHeightMap(const char* path);

    // Returns the red component of a pixel
    inline unsigned int GetPixelR(int x, int y) {
        return heightMap[(x*3) / width + (height / this->height * (y*3))];
    }
    // Returns the green component of a pixel
    inline unsigned int GetPixelG(int x, int y) {
        return heightMap[(x*3)+height*(y*3)+1];
    }
    // Returns the blue component of a pixel
    inline unsigned int GetPixelB(int x, int y) {
        return heightMap[(x*3)+height*(y*3)+2];
    }
    
    void Draw(Shader& s); 
    Transform& getTransform();

private:
    // data
    unsigned int xSegments;
    unsigned int zSegments;
    
    unsigned char* heightMap;
    int width;
    int height;
    int nrComponents;

    int* heightData;

    Mesh* m;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture t;
    Transform transform;
};

#endif
