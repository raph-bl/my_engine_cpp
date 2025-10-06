#ifndef CAMERA_H
#define CAMERA_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;
	bool isInCreative = false;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform);
	void Inputs(GLFWwindow* window);

	float get_X() {
		return Position.x;
	}

	float get_Y() {
		return Position.y;
	}

	float get_Z() {
		return Position.z;
	}

	void set_Y(float v) {
		Position.y = v;
	}

	bool get_player_gamemode() {
		return isInCreative;
	}

	void set_player_gamemode(bool b) {
		isInCreative = b;
	}

	glm::vec3 getPosition() const { return Position; }
	glm::vec3 getFront() const { return Orientation; }
};
#endif