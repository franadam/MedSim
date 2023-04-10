#pragma once
#ifndef _PHYSICS_
#define _PHYSICS_

// System Headers
#include "glitter.hpp"

//Bullet
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "bullet/BulletSoftBody/btSoftBodyHelpers.h"
//
// Local Headers
#include "Scene.hpp"
#include "Camera.hpp"

namespace physics
{

	class Physics
	{

	public:
		Physics(resource::Scene* scene);
		Physics() = default;
		~Physics();

		void update(float dt);
		void DrawBox(const btVector3& halfSize, const btVector3& color = btVector3(1.0f, 1.0f, 1.0f));
		//void createRigidBody(const glm::vec3& initialPosition, const float& mass);
		btRigidBody* createRigidBody(const glm::vec3& initialPosition, const float& mass);
		void creatSoftBody(const glm::vec3& initialPosition, const float& mass);
		btRigidBody* getGround();

	private:
		resource::Scene* m_scene;
		float m_friction;
		float m_restitution;
		btBroadphaseInterface* m_broadphase;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_world;
		btSoftRigidDynamicsWorld* m_softBodyWorld;
		std::vector<btRigidBody*> m_bodies;
		btDefaultMotionState* m_motionState;
		btRigidBody* m_ground;

		//// a pointer to our world, typecast into its soft body type
		//btSoftRigidDynamicsWorld* m_SoftBodyWorld;
		//
		//// the soft body world info. Needed for proper contact generation
		//btSoftBodyWorldInfo  m_softBodyWorldInfo;
		void initPhysics();
	};
}

#endif