#pragma once

#ifndef _CONTROLLER_
#define _CONTROLLER_

//Bullet
//#include "bullet/btBulletCollisionCommon.h"
//#include "bullet/btBulletDynamicsCommon.h"

// Local Headers
#include "Gui.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

class Controller
{
public:
	Controller(renderer::Camera* camera, resource::Scene* scene);
	Controller() = default;
	~Controller();

	void modelsInteractions();
	void update();

private:
	resource::Scene* m_scene;
	renderer::Camera* m_camera;
	glm::mat4 posTargetPosition;
	int m_id_model = -1;
	Ray m_ray;

	void testIntersect(Ray ray);
	void moveModel();
};

#endif