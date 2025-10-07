#include "Camera.h"
#include "entities/Cube.h"
#include <cmath>
#include <GL/gl.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, const char *uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(view));
}

void Camera::Inputs(GLFWwindow *window, std::vector<std::unique_ptr<Cube>>& cubes)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	{
		static bool key1_was_pressed = false;
		bool key1_is_pressed = (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS);
		if (key1_is_pressed && !key1_was_pressed)
		{
			set_player_gamemode(!isInCreative);
		}
		key1_was_pressed = key1_is_pressed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}
	static bool cursorCaptured = false;
	{
		static bool escWasPressed = false;
		bool escIsPressed = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
		if (escIsPressed && !escWasPressed && cursorCaptured)
		{
			// Release cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorCaptured = false;
			firstClick = true;
		}
		escWasPressed = escIsPressed;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !cursorCaptured)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPos(window, (width / 2), (height / 2));
		cursorCaptured = true;
		firstClick = false;
	}
	{
		static bool rightClickWasPressed = false;
		bool rightClickIsPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
		if (rightClickIsPressed && !rightClickWasPressed && cursorCaptured && get_player_gamemode() == true)
		{
			glm::vec3 camPos = getPosition();
			glm::vec3 camFront = getFront();
			glm::vec3 cubePos = camPos + camFront * 5.0f;
			cubes.emplace_back(std::make_unique<Cube>(cubePos));
			cout << "Cube created at: (" << cubePos.x << ", " << cubePos.y << ", " << cubePos.z << ")" << endl;
		}
		rightClickWasPressed = rightClickIsPressed;
	}
	{
		static bool leftClickWasPressed = false;
		bool leftClickIsPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
		if (leftClickIsPressed && !leftClickWasPressed && cursorCaptured && get_player_gamemode() == true)
		{
			glm::vec3 camPos = getPosition();
			glm::vec3 camFront = getFront();
			glm::vec3 cubePos = camPos + camFront * 5.0f;
			cubes.emplace_back(std::make_unique<Cube>(cubePos));
			cout << "Cube deleted at: (" << cubePos.x << ", " << cubePos.y << ", " << cubePos.z << ")" << endl;
		}
		leftClickWasPressed = leftClickIsPressed;
	}

	// Handle mouse look when cursor is captured
	if (cursorCaptured)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
}