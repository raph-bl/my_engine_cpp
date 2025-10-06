#ifndef CUBE_H
#define CUBE_H
#include <glm/glm.hpp>

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    
    void setupMesh();
    
public:
    Cube(glm::vec3 pos = glm::vec3(0.0f));
    ~Cube();
    
    void setPosition(glm::vec3 pos);
    void move(glm::vec3 offset);
    void setScale(glm::vec3 s);
    void setRotation(glm::vec3 rot);
    
    glm::mat4 getModelMatrix();
    void draw(unsigned int shaderProgram);
};
#endif