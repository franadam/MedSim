#pragma once

#ifndef _CONTROLLER_
#define _CONTROLLER_


// Local Headers
#include "Gui.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

class Controller
{
public:
	Controller(Camera* camera, Scene* scene);
	Controller() = default;

	void modelsInteractions();
	void update();

private:
	Scene* m_scene;
	Camera* m_camera;
	glm::mat4 posTargetPosition;
	int m_id_model = -1;
	Ray m_ray;

	void testIntersect(Ray ray);
	void moveModel();
};

#endif