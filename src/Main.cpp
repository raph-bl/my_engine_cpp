#include <GL/glew.h>
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "WindowManager.h"
#include "Camera.h"
#include "../include/TextManager.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <thread>
#include <vector>
#include <string>
#include <memory>
#include "entities/Cube.h"

using namespace std;

GLFWwindow* window;
static Camera *cam          = nullptr;
WindowManager *manager  = WindowManager::getInstance();
void update_loop() {
    static float angle = 0.0f;
    static const float ANGULAR_SPEED = 45.0f;
    static const int TARGET_FPS = 60;

    using clock = std::chrono::steady_clock;
    static clock::time_point last = clock::now();
    auto now = clock::now();
    std::chrono::duration<float> elapsed = now - last;
    float delta = elapsed.count();
    last = now;

    if (delta < 0.0f) delta = 0.0f;
    if (delta > 0.1f) delta = 0.1f;

    angle += ANGULAR_SPEED * delta;
    if (angle >= 360.0f) angle -= 360.0f;

    const float targetSec = 1.0f / float(TARGET_FPS);
    if (delta < targetSec) {
        std::this_thread::sleep_for(std::chrono::duration<float>(targetSec - delta));
        last = clock::now();
    }
    if (cam) {
        cam->Inputs(window);
        cam->Matrix(45.0f, 0.1f, 100.0f, "camera");
    }
    // cout << TARGET_FPS << endl;
}

void render_loop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    
    std::vector<std::unique_ptr<Cube>> cubes;
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(0.0f, 0.0f, 0.0f)));
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(1.0f, 0.0f, 0.0f)));
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(-1.0f, 0.0f, 0.0f)));
    
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(2.0f, 0.0f, 0.0f)));
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(2.0f, 1.0f, 0.0f)));
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(2.0f, 2.0f, 0.0f)));
    
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(2.0f, 2.0f, 1.0f)));
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(2.0f, 2.0f, 2.0f)));
    cubes.emplace_back(std::make_unique<Cube>(glm::vec3(2.0f, 2.0f, 3.0f)));
    
    if (cam->get_player_gamemode()) {
        if (cam) {
            glm::vec3 camPos = cam->getPosition();
            glm::vec3 camFront = cam->getFront();
            glm::vec3 cubePos = camPos + camFront * 5.0f;
            cubes.emplace_back(std::make_unique<Cube>(cubePos));
        }
    }


    for (auto &c : cubes) {
        if (!c) continue;
        // c->setRotation(glm::vec3(0.0f, (float)glfwGetTime() * 50.0f, 0.0f));
        c->draw(0);
    }

    glBegin(GL_LINES);
    glColor3f(0.85f, 0.85f, 0.85f);
    for (GLfloat i = -10.5f; i <= 10.5f; i += 0.25f) {
        glVertex3f(i, 0, 10.5f);
        glVertex3f(i, 0, -10.5f);
        glVertex3f(10.5f, 0, i);
        glVertex3f(-10.5f, 0, i);
    }
    glEnd();

    std::string labelTitle = "My_engine - X: " + std::to_string(round(cam->get_X())) + "/Y: " + std::to_string(round(cam->get_Y())) + "/Z: " + std::to_string(round(cam->get_Z()));
    std::string labelMod = "Creative mod : " + std::to_string(cam->get_player_gamemode()); 
    drawTextOnScreen(labelTitle, 10, 20, manager->getWidth(), manager->getHeight());
    drawTextOnScreen(labelMod, 10, 40, manager->getWidth(), manager->getHeight());
}

void init() {
    cam = new Camera(manager->getWidth(), manager->getHeight(), glm::vec3(0.0f, 0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(58.0f/255.0f, 58.0f/255.0f, 58.0f/255.0f, 1.0f);
    glViewport(0, 0, manager->getWidth(), manager->getHeight());
}

int main(int argc, char **argv) {
    int win_width           = manager->getWidth();
    int win_height          = manager->getHeight();
    string window_title     = "My_engine / " + to_string(win_width) + "x" + to_string(win_height);

    if (!glfwInit()) {
        cerr << "!> Failed to initialize GLFW." << endl;
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(manager->getWidth(), manager->getHeight(), window_title.c_str(), NULL, NULL); 
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread. It can also detach the current context from the calling thread without making a new one current by passing in NULL.

    glewExperimental        = GL_TRUE;
    GLenum glewErr          = glewInit();
    if(glewErr != GLEW_OK) {
        cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewErr) << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    // Initialize GLUT (required before using glutBitmapCharacter)
    // We only use GLUT for bitmap fonts here; GLFW is still the window/context provider.
    {
        int glut_argc = 1;
        char *glut_argv[1] = { (char*)"my_engine" };
        glutInit(&glut_argc, glut_argv);
    }
    glfwSetWindowTitle(window, window_title.c_str());

    init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        update_loop();
        render_loop();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}