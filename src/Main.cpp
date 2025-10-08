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

GLFWwindow *window;
static Camera *cam = nullptr;
WindowManager *manager = WindowManager::getInstance();
std::vector<std::unique_ptr<Cube>> cubes;
void update_loop()
{
    static float angle = 0.0f;
    static const float ANGULAR_SPEED = 45.0f;
    static const int TARGET_FPS = 60;

    using clock = std::chrono::steady_clock;
    static clock::time_point last = clock::now();
    auto now = clock::now();
    std::chrono::duration<float> elapsed = now - last;
    float delta = elapsed.count();
    last = now;

    if (delta < 0.0f)
        delta = 0.0f;
    if (delta > 0.1f)
        delta = 0.1f;

    angle += ANGULAR_SPEED * delta;
    if (angle >= 360.0f)
        angle -= 360.0f;

    const float targetSec = 1.0f / float(TARGET_FPS);
    if (delta < targetSec)
    {
        std::this_thread::sleep_for(std::chrono::duration<float>(targetSec - delta));
        last = clock::now();
    }
    if (cam)
    {
        cam->Inputs(window, cubes);
        cam->Matrix(45.0f, 0.1f, 100.0f, "camera");
    }
    // cout << TARGET_FPS << endl;
}

void render_loop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glm::vec3 camPos = cam->getPosition();
    glm::vec3 camFront = cam->getFront();
    glm::vec3 previewPos = camPos + camFront * 5.0f;
    static glm::vec3 prev_pos = camPos;
    bool moving = prev_pos != camPos;
    
    if (moving) {
        cout << "Player moved from (" 
             << prev_pos.x << ", " << prev_pos.y << ", " << prev_pos.z 
             << ") to (" 
             << camPos.x << ", " << camPos.y << ", " << camPos.z 
             << ")" << endl;        
        bool collisionDetected = false;
        int closestCubeIndex = -1;
        float closestDistance = 1.5f;
        
        for (size_t i = 0; i < cubes.size(); i++) {
            if (!cubes[i]) continue;
            
            glm::vec3 cubePos = cubes[i]->getPosition();
            float distanceFromCamera = cam->getDistanceToCube(cubePos);

            if (distanceFromCamera < closestDistance) {
                cout << "COLLISION detected with cube " << i << " at distance " << distanceFromCamera << endl;
                cam->setPosition(prev_pos);
                collisionDetected = true;
                closestCubeIndex = static_cast<int>(i);
                break;
            }
        }
        
        if (collisionDetected) {
            cout << "Movement cancelled due to collision with cube " << closestCubeIndex << endl;
        } else {
            prev_pos = camPos;
        }
    } else {
        prev_pos = camPos;
    }

    if (cam->get_player_gamemode())
    {
        if (cam)
        {   
            glPushMatrix();
            glTranslatef(previewPos.x, previewPos.y, previewPos.z);

            GLfloat prevColor[4];
            glGetFloatv(GL_CURRENT_COLOR, prevColor);
            glColor3f(0.2f, 0.8f, 0.2f);
            glutWireCube(1.0f);
            glColor4fv(prevColor);

            glPopMatrix();
        }
    }

    for (auto &c : cubes)
    {
        if (!c)
            continue;
        // c->setRotation(glm::vec3(0.0f, (float)glfwGetTime() * 50.0f, 0.0f));
        c->draw(0);
    }

    // if (cubes.empty())
    // {
    //     int i = 0;
    //     for (int x = -2; x <= 2; ++x)
    //     {
    //         for (int y = -2; y <= 2; ++y)
    //         {
    //             for (int z = -2; z <= 2; ++z)
    //             {
    //                 // Set cube color to red before adding (if Cube supports setColor)
    //                 cubes.emplace_back(std::make_unique<Cube>(glm::vec3(float(x), float(y), float(z))));
    //                 cubes.back()->setColor(1.0f, 0.0f, 0.0f);
    //                 i++;
    //             }
    //         }
    //     }
    // }

    glBegin(GL_LINES);
    glColor3f(0.85f, 0.85f, 0.85f);
    for (GLfloat i = -10.5f; i <= 10.5f; i += 0.25f)
    {
        glVertex3f(i, 0, 10.5f);
        glVertex3f(i, 0, -10.5f);
        glVertex3f(10.5f, 0, i);
        glVertex3f(-10.5f, 0, i);
    }
    glEnd();

    std::string labelTitle = "My_engine - X: " + std::to_string(round(cam->get_X())) + "/Y: " + std::to_string(round(cam->get_Y())) + "/Z: " + std::to_string(round(cam->get_Z()));
    std::string labelMod = "Creative mod: " + std::string(cam->get_player_gamemode() ? "Creative" : "Survival");
    std::string labelCubes = "Cubes: " + std::to_string(cubes.size());
    // cout << "Cubes count: " << cubes.size() << endl;
    drawTextOnScreen(labelTitle, 10, 20, manager->getWidth(), manager->getHeight());
    drawTextOnScreen(labelMod, 10, 40, manager->getWidth(), manager->getHeight());
    drawTextOnScreen(labelCubes, 10, 60, manager->getWidth(), manager->getHeight());
}

void init()
{
    cam = new Camera(manager->getWidth(), manager->getHeight(), glm::vec3(0.0f, 0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(58.0f / 255.0f, 58.0f / 255.0f, 58.0f / 255.0f, 1.0f);
    glViewport(0, 0, manager->getWidth(), manager->getHeight());
}

int main(int argc, char **argv)
{
    int win_width = manager->getWidth();
    int win_height = manager->getHeight();
    string window_title = "My_engine / " + to_string(win_width) + "x" + to_string(win_height);

    if (!glfwInit())
    {
        cerr << "!> Failed to initialize GLFW." << endl;
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(manager->getWidth(), manager->getHeight(), window_title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread. It can also detach the current context from the calling thread without making a new one current by passing in NULL.

    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK)
    {
        cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewErr) << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    // Initialize GLUT (required before using glutBitmapCharacter)
    // We only use GLUT for bitmap fonts here; GLFW is still the window/context provider.
    {
        int glut_argc = 1;
        char *glut_argv[1] = {(char *)"my_engine"};
        glutInit(&glut_argc, glut_argv);
    }
    glfwSetWindowTitle(window, window_title.c_str());

    init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        update_loop();
        render_loop();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
