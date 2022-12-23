#include "SDLGraphicsProgram.hpp"
#include "model.h"
#include "song.hpp"
#include "visualizer.hpp"
#include "square.hpp"
#include "shader.h"
#include "entity.h"

#define NUM_BARS 100
#define SCREEN_W 1280
#define SCREEN_H 720

Square* sq;
Model* monkey;
Entity* m;

Shader* sPtr, *s2Ptr;

glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)SCREEN_W)/((float)SCREEN_H), 0.1f, 20.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
void callbackBars(double* vals, unsigned int numBuckets) {
    for(int i = 0; i < numBuckets; i++) {
        Transform& t = sq[i].getTransform();
        t.setLocalScale(glm::vec3(2.0f / NUM_BARS, vals[i], 1.0f));
        t.setLocalPosition(glm::vec3(1.0f / NUM_BARS + 2.0f / NUM_BARS * i - 1.0f, (vals[i] / 2) - 1.0f, 0.0f));
        t.computeModelMatrix();
    }
}

void callbackModelScale(double* vals, unsigned int numBuckets) {
    Transform& t = m->transform;
    //std::cout << "Monkey: " << log10(10 * vals[0]) * 0.7f << " " << log10(10 * vals[1]) << std::endl;
    //t.setLocalScale(glm::vec3(log10(10 * vals[0]) * 0.7f, log10(10 * vals[1]), 1.0f));
    //std::cout << "Monkey: " << vals[0] << " " << vals[1] << std::endl;
    t.setLocalPosition(glm::vec3(0.0f, 0.0f, -10.0f));
    t.setLocalScale(glm::vec3(3 * vals[0], 3 * vals[1], 1.0f));
    t.computeModelMatrix();
}

void loopCallback() {
    s2Ptr->use();
    s2Ptr->setMat4("projection", projection);
    s2Ptr->setMat4("view", view); 
    s2Ptr->setVec3("pointLights.lightColor",glm::vec3(1.0f,1.0f,1.0f));
    s2Ptr->setVec3("pointLights.lightPos", glm::vec3(0.0f, 1.0f, -1.0f));
    s2Ptr->setFloat("pointLights.ambientIntensity",0.9f);
    s2Ptr->setFloat("pointLights.specularStrength",0.1f);
    s2Ptr->setFloat("pointLights.constant",1.0f);
    s2Ptr->setFloat("pointLights.linear",0.01f);
    s2Ptr->setFloat("pointLights.quadratic",0.001f);

    unsigned int total = 0;
    m->drawSelfAndChild(*s2Ptr, total);

    sPtr->use();
    for(int i = 0; i < NUM_BARS; i++) {
        sq[i].Draw(*sPtr);
    }
}

bool runMonkeyDemo(char* songPath) {
    // Create an instance of an object for a SDLGraphicsProgram
    SDLGraphicsProgram mySDLGraphicsProgram(SCREEN_W, SCREEN_H);
    
    // Print out some useful information
    mySDLGraphicsProgram.GetOpenGLVersionInfo();

    Texture box("resources/bar.png", "./");
    sq = (Square*) malloc(sizeof(Square) * NUM_BARS);
    for(int i = 0; i < NUM_BARS; i++) {
        new (&sq[i]) Square(box);// = Square(box);
    }
    monkey = new Model("resources/monkey.obj");
    m = new Entity(*monkey);
    Shader s("./shaders/simpleVert.glsl", "./shaders/simpleFrag.glsl"); 
    
    Shader s2("./shaders/vert_untextured.glsl", "./shaders/frag_untextured.glsl"); 
    sPtr = &s;
    s2Ptr = &s2;
    Song song(songPath);
    VisualizerCallback callbacks[] = {&callbackBars, &callbackModelScale, nullptr};
    unsigned int numBuckets[] = {NUM_BARS, 2};
    Visualizer v(callbacks, song.getSampleFrameLength() * 2, numBuckets);
    v.setSong(&song);
    song.play();

    mySDLGraphicsProgram.v = &v;
    
    // Run our program forever
    mySDLGraphicsProgram.Loop(&loopCallback);

    // When our program ends, it will exit scope, the
    // destructor will then be called and clean up the program.
    return true;
}
