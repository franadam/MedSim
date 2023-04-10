#include "../Headers/Physics.hpp"

namespace physics
{
	Physics::Physics( resource::Scene* scene)
	{
		initPhysics();
	}

	Physics::~Physics()
	{
		delete m_world;
		delete m_solver;
		delete m_broadphase;
		delete m_dispatcher;
		delete m_collisionConfiguration;
	}

	void Physics::update(float dt)
	{
		// check if the world object exists
		if (m_world) {
			// step the simulation through time. This is called
			// every update and the amount of elasped time was 
			// determined back in ::Idle() by our clock object.
			m_world->stepSimulation(dt);
		}
	}

	btRigidBody* Physics::getGround()
	{
		return m_ground;
	}

	void Physics::initPhysics() {

		m_broadphase = new btDbvtBroadphase();
		m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();  //new btDefaultCollisionConfiguration(); ////rigidbody
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		m_solver = new btSequentialImpulseConstraintSolver;
		m_world = new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration); //new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig); ////rigidbody
		m_world->setGravity(btVector3(0, -10, 0));

		// typecast the world and store it for future usage
		m_softBodyWorld = (btSoftRigidDynamicsWorld*)m_world;

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
		m_motionState = new btDefaultMotionState(transform);
		btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		btRigidBody::btRigidBodyConstructionInfo info(0.0, m_motionState, plane);
		m_ground = new btRigidBody(info);
		m_world->addRigidBody(m_ground);
	}

	btRigidBody* Physics::createRigidBody(const glm::vec3& initialPosition, const float& mass) {
		// create a box shape of size (1,1,1)
		btBoxShape* pBoxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)); //put aabb dim
		// give our box an initial position of (0,0,0)
		btTransform transform;

		transform.setIdentity();
		transform.setOrigin(btVector3(initialPosition.x, initialPosition.y, initialPosition.z));
		//transform.setFromOpenGLMatrix(position);
		// create a motion state
		m_motionState = new btDefaultMotionState(transform);
		// create the rigid body construction info object, giving it a 
		// mass of 1, the motion state, and the shape
		btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, m_motionState, pBoxShape);
		btRigidBody* pRigidBody = new btRigidBody(rbInfo);
		// inform our world that we just created a new rigid body for 
		// it to manage
		m_world->addRigidBody(pRigidBody);

		return pRigidBody;
	}


	//void Physics::creatSoftBody(const btScalar* position) {
	//	// create a ground plane
	//	//CreateGameObject(new btBoxShape(btVector3(1, 50, 50)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));
	//
	//	// create a soft 'ball' with 128 sides and a radius of 3
	//	btSoftBody* pSoftBody = btSoftBodyHelpers::CreateEllipsoid(m_softBodyWorldInfo, btVector3(0, 0, 0), btVector3(3, 3, 3), 128);
	//
	//	// set the body's position
	//	pSoftBody->translate(btVector3(0, 15, 0));
	//
	//	// set the 'volume conservation coefficient'
	//	pSoftBody->m_cfg.kVC = 0.5;
	//
	//	// set the 'linear stiffness'
	//	pSoftBody->m_materials[0]->m_kLST = 0.5;
	//
	//	// set the total mass of the soft body
	//	pSoftBody->setTotalMass(5);
	//
	//	// tell the soft body to initialize and
	//	// attempt to maintain the current pose
	//	pSoftBody->setPose(true, false);
	//
	//	// add the soft body to our world
	//	m_pSoftBodyWorld->addSoftBody(pSoftBody);
	//}
}
