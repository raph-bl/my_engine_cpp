#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class CameraManager
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	CameraManager(int width, int height, glm::vec3 position);

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
};
#endif