#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Camera.h"
#include "WindowManager.h"

void render_world(float size);
void render_text(WindowManager* manager, Camera *cam, const std::vector<std::unique_ptr<Cube>>& cubes);

#endif // RENDERMANAGER_H