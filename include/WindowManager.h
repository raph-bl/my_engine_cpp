#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

class WindowManager {
    private:
        int width, height;
        static WindowManager *instancePtr;
        WindowManager():
            width(1500),
            height(800) {}

    public:
        WindowManager(const WindowManager &obj)         = delete;
        WindowManager &operator=(const WindowManager &) = delete;

        static WindowManager *getInstance();

        void setWindowSize(int width, int height) {
            this->width     = width;
            this->height    = height;
        }

        int getWidth() {
            return width;
        }

        int getHeight() {
            return height;
        }
};

#endif