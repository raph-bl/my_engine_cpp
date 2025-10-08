#ifndef SPHERE_H
#define SPHERE_H

class Sphere {

    private:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;

    public:
        Sphere(glm::vec3 pos = glm::vec3(0.0f));
        ~Sphere();
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

        
        void setPosition(glm::vec3 pos);
        void move(glm::vec3 offset);
        void setScale(glm::vec3 s);
        void setRotation(glm::vec3 rot);

        void setColor(glm::vec3 color);
        void setColor(float r, float g, float b);    
        
        glm::vec3 getColor() const { return color; }
        glm::vec3 getPosition() const { return position; }
        glm::mat4 getModelMatrix();
        void draw(unsigned int shaderProgram);
};
#endif