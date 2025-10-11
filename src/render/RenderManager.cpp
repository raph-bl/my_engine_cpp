#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "WindowManager.h"
#include "Camera.h"
#include "TextManager.h"
#include "entities/Cube.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

void render_artifacts() {
    
}

void render_world(float size) {
    glBegin(GL_LINES);
    glColor3f(0.85f, 0.85f, 0.85f);
    for (GLfloat i = -size; i <= size; i += 0.25f) {
        glVertex3f(i, 0, size);
        glVertex3f(i, 0, -size);
        glVertex3f(size, 0, i);
        glVertex3f(-size, 0, i);
    }
    glEnd();
}

void render_text(WindowManager* manager, Camera *cam, const std::vector<std::unique_ptr<Cube>>& cubes) {
    std::string labelTitle = "My_engine - X: " + std::to_string(round(cam->get_X())) + "/Y: " + std::to_string(round(cam->get_Y())) + "/Z: " + std::to_string(round(cam->get_Z()));
    std::string labelMod = "Creative mod: " + std::string(cam->get_player_gamemode() ? "Creative" : "Survival");
    std::string labelCubes = "Cubes: " + std::to_string(cubes.size());
    // cout << "Cubes count: " << cubes.size() << endl;
    drawTextOnScreen(labelTitle, 10, 20, manager->getWidth(), manager->getHeight());
    drawTextOnScreen(labelMod, 10, 40, manager->getWidth(), manager->getHeight());
    drawTextOnScreen(labelCubes, 10, 60, manager->getWidth(), manager->getHeight());
}