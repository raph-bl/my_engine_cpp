#include "WindowManager.h"

WindowManager* WindowManager::instancePtr = nullptr;

WindowManager* WindowManager::getInstance() {
    if (!instancePtr) instancePtr = new WindowManager();
    return instancePtr;
}
