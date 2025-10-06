#include "entities/Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

Cube::Cube(glm::vec3 pos) 
    : position(pos), scale(1.0f), rotation(0.0f) {
    setupMesh();
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::setupMesh() {
    float vertices[] = {
        // Positions
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };
    
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Face avant
        1, 5, 6, 6, 2, 1, // Face droite
        5, 4, 7, 7, 6, 5, // Face arrière
        4, 0, 3, 3, 7, 4, // Face gauche
        3, 2, 6, 6, 7, 3, // Face haut
        4, 5, 1, 1, 0, 4  // Face bas
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Cube::setPosition(glm::vec3 pos) {
    position = pos;
}

void Cube::move(glm::vec3 offset) {
    position += offset;
}

void Cube::setScale(glm::vec3 s) {
    scale = s;
}

void Cube::setRotation(glm::vec3 rot) {
    rotation = rot;
}

glm::mat4 Cube::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);
    return model;
}

void Cube::draw(unsigned int shaderProgram) {
    glm::mat4 model = getModelMatrix();
    if (shaderProgram != 0) {
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        if (modelLoc != -1) {
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    } else {
        glPushMatrix();
        glMultMatrixf(&model[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glPopMatrix();
    }
}