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

// Local Headers
#include "Scene.hpp"
#include "Camera.hpp"

namespace physics
{

	class Physics
	{

	public:
		std::vector<btRigidBody*> m_bodies;
		
		Physics();
		//Physics() = default;
		~Physics();

		void update(float dt);
		//void DrawBox(const btVector3& halfSize, const btVector3& color = btVector3(1.0f, 1.0f, 1.0f));
		//void createRigidBody(const glm::vec3& initialPosition, const float& mass);
		btRigidBody* createRigidBody(const glm::vec3& pInitialPosition, const float& pMass, btCollisionShape* pShape);
		btRigidBody* createRigidBody(const glm::vec3& pInitialPosition, const float& pMass);
		void creatSoftBody(const glm::vec3& initialPosition, const float& mass);
		btRigidBody* getGround();
		void DrawBox(const btVector3& halfSize, const btVector3& color);

	private:
		float m_friction;
		float m_restitution;
		btBroadphaseInterface* m_broadphase;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_world;
		btSoftRigidDynamicsWorld* m_softBodyWorld; // a pointer to our world, typecast into its soft body type
		btSoftBodyWorldInfo  m_softBodyWorldInfo; // the soft body world info. Needed for proper contact generation
		btDefaultMotionState* m_motionState;
		btRigidBody* m_ground;
		

		// mass in kg
		float mass; 
		// position in m
		glm::vec3 pos;
		// velocity in m/s
		glm::vec3 velocity;
		// acceleration in m/s^2
		glm::vec3 acceleration;
		// dimensions of object
		glm::vec3 size;
		// rotation in Euler angles
		glm::vec3 rot;

		void initPhysics();
	};
}

#endif