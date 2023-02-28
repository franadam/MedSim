#pragma once
#ifndef _MOUSE_PICKER_
#define _MOUSE_PICKER_

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Std. Includes
#include <vector>

//Matrix
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

/// Local Headers
//#include "glitter.hpp"
#include "Camera.hpp"
#include "Gui.hpp"

class MousePicker
{
private:
	glm::vec3 m_currentRay;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	Camera m_camera;
	double m_mouseX;
	double m_mouseY;

public:
	MousePicker(Camera cam, glm::mat4 projection);
	MousePicker() = default;

	glm::vec3 getCurrentRay();

	void update();

	glm::vec2 getScreenCoordinate();
	glm::vec3 normalisedCoordinates(double xpos, double ypos);
	glm::vec4 eyeCoordinates(glm::vec4  homogeneousClip);
	glm::vec3 worldCoordinates(glm::vec4  ray_eye);
	glm::vec3 computeCurrentRay();
	glm::vec3 getPointOnRay(glm::vec3 ray, float distance);
	glm::vec3 binarySearch(int count, float start, float finish, glm::vec3 ray);
	bool intersectionInRange(float start, float finish, glm::vec3 ray);
};

#endif 