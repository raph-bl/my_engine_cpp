#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <string>

// Draw simple bitmap text on screen at pixel coords (x,y).
// screenW/screenH are used to set an orthographic projection for overlay.
void drawTextOnScreen(const std::string &text, int x, int y, int screenW, int screenH);

#endif
