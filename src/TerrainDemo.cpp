#include "SDLGraphicsProgram.hpp"
#include "model.h"
#include "song.hpp"
#include "visualizer.hpp"
#include "shader.h"
#include "entity.h"
#include "Terrain.hpp"

#define NUM_BARS 100
#define SCREEN_W 1280
#define SCREEN_H 720

Shader* sTexturePtr;
Terrain* terrainPtr;
glm::mat4 projectionTerrain = glm::perspective(glm::radians(45.0f), ((float)SCREEN_W)/((float)SCREEN_H), 0.1f, 100.0f);
glm::mat4 viewTerrain = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));  

void callbackTerrain(double* vals, unsigned int numBuckets) {
    Transform& t = terrainPtr->getTransform();
    t.setLocalPosition(glm::vec3(0.0f, -2.0f, -10.0f));
    //t.setLocalScale(glm::vec3(3 * vals[0], 3 * vals[1], 1.0f));
    t.computeModelMatrix();
}

void loopCallbackTerrain() {
    sTexturePtr->use();
    sTexturePtr->setMat4("projection", projectionTerrain);
    sTexturePtr->setMat4("view", viewTerrain); 
    sTexturePtr->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f));
    sTexturePtr->setVec3("pointLights.lightColor",glm::vec3(1.0f,1.0f,1.0f));
    sTexturePtr->setVec3("pointLights.lightPos", glm::vec3(0.0f, 1.0f, -1.0f));
    sTexturePtr->setFloat("pointLights.ambientIntensity",0.9f);
    sTexturePtr->setFloat("pointLights.specularStrength",0.1f);
    sTexturePtr->setFloat("pointLights.constant",1.0f);
    sTexturePtr->setFloat("pointLights.linear",0.01f);
    sTexturePtr->setFloat("pointLights.quadratic",0.001f);


    unsigned int total = 0;

    terrainPtr->Draw(*sTexturePtr);
}


bool runTerrainDemo(char* songPath) {
    // Create an instance of an object for a SDLGraphicsProgram
    SDLGraphicsProgram mySDLGraphicsProgram(SCREEN_W, SCREEN_H);
    
    // Print out some useful information
    mySDLGraphicsProgram.GetOpenGLVersionInfo();

    Terrain terrain = Terrain(512, 512, "terrain.bmp", "colormap.bmp");
    terrainPtr = &terrain;

    Shader s("./shaders/vertTexture.glsl", "./shaders/fragTexture.glsl"); 
    sTexturePtr = &s;
    Song song(songPath);
    VisualizerCallback callbacks[] = {&callbackTerrain, nullptr};
    unsigned int numBuckets[] = {NUM_BARS};
    Visualizer v(callbacks, song.getSampleFrameLength(), numBuckets);
    v.setSong(&song);
    song.play();

    mySDLGraphicsProgram.v = &v;
    //v.play();
    
    // Run our program forever
    mySDLGraphicsProgram.Loop(&loopCallbackTerrain);

    // When our program ends, it will exit scope, the
    // destructor will then be called and clean up the program.
    return true;
}
