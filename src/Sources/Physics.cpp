#include "../Headers/Physics.hpp"

namespace physics
{
	Physics::Physics()
	{
		std::cout << "\n Physics::Physics() ... \n";
		initPhysics();
		m_friction= 5;
		m_restitution= 5;

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
		//std::cout << "\n Physics::update() ... \n";
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

		std::cout << "\n Physics::initPhysics() ... \n";
		m_broadphase = new btDbvtBroadphase();
		m_collisionConfiguration = new btDefaultCollisionConfiguration();// new btSoftBodyRigidBodyCollisionConfiguration();   ////rigidbody
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		m_solver = new btSequentialImpulseConstraintSolver;
		m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);// new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration); // ////rigidbody
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
		m_bodies.push_back(m_ground);

	}

	btRigidBody* Physics::createRigidBody(const glm::vec3& pInitialPosition, const float& pMass) {
		std::cout << "\n Physics::createRigidBody() ... \n";
		// create a box shape of size (1,1,1)
		btBoxShape* pBoxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)); //put aabb dim

		//// calculate the local inertia
		//btVector3 localInertia(0, 0, 0);
		//
		//// objects of infinite mass can't
		//// move or rotate
		//if (pMass != 0.0f) {
		//	pBoxShape->calculateLocalInertia(pMass, localInertia);
		//}
		//
		// give our box an initial position of (0,0,0)
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(pInitialPosition.x, pInitialPosition.y, pInitialPosition.z));
		//transform.setFromOpenGLMatrix(position);
		// create a motion state
		m_motionState = new btDefaultMotionState(transform);
		// create the rigid body construction info object, giving it a 
		// mass of 1, the motion state, and the shape
		btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, m_motionState, pBoxShape);
		btRigidBody* rigidBody = new btRigidBody(rbInfo);

		// inform our world that we just created a new rigid body for it to manage
		m_world->addRigidBody(rigidBody);
		//rigidBody->forceActivationState(ACTIVE_TAG);
		//rigidBody->activate();
		m_bodies.push_back(rigidBody);

		return rigidBody;
	}


	void Physics::creatSoftBody(const glm::vec3& initialPosition, const float& mass) {
		std::cout << "\n Physics::creatSoftBody() ... \n";
		// create a ground plane
		//CreateGameObject(new btBoxShape(btVector3(1, 50, 50)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));
	
		// create a soft 'ball' with 128 sides and a radius of 3
		btSoftBody* pSoftBody = btSoftBodyHelpers::CreateEllipsoid(m_softBodyWorldInfo, btVector3(0, 0, 0), btVector3(3, 3, 3), 128);
	
		// set the body's position
		pSoftBody->translate(btVector3(0, 15, 0));
	
		// set the 'volume conservation coefficient'
		pSoftBody->m_cfg.kVC = 0.5;
	
		// set the 'linear stiffness'
		pSoftBody->m_materials[0]->m_kLST = 0.5;
	
		// set the total mass of the soft body
		pSoftBody->setTotalMass(mass);
	
		// tell the soft body to initialize and
		// attempt to maintain the current pose
		pSoftBody->setPose(true, false);
	
		// add the soft body to our world
		m_softBodyWorld->addSoftBody(pSoftBody);
	}


	void Physics::DrawBox(const btVector3& halfSize, const btVector3& color) {
		float halfWidth = halfSize.x();
		float halfHeight = halfSize.y();
		float halfDepth = halfSize.z();

		// set the object's color
		glColor3f(color.x(), color.y(), color.z());

		// create the vertex positions
		btVector3 vertices[8] = {
		btVector3(halfWidth,  halfHeight,  halfDepth),
		btVector3(-halfWidth,  halfHeight,  halfDepth),
		btVector3(halfWidth, -halfHeight,  halfDepth),
		btVector3(-halfWidth, -halfHeight,  halfDepth),
		btVector3(halfWidth,  halfHeight, -halfDepth),
		btVector3(-halfWidth,  halfHeight, -halfDepth),
		btVector3(halfWidth, -halfHeight, -halfDepth),
		btVector3(-halfWidth, -halfHeight, -halfDepth) };

		// create the indexes for each triangle, using the
		// vertices above. Make it static so we don't waste 
		// processing time recreating it over and over again
		static int indices[36] = {
			0,1,2,
			3,2,1,
			4,0,6,
			6,0,2,
			5,1,4,
			4,1,0,
			7,3,1,
			7,1,5,
			5,4,7,
			7,4,6,
			7,2,3,
			7,6,2 };

		// start processing vertices as triangles
		glBegin(GL_TRIANGLES);

		// increment the loop by 3 each time since we create a
		// triangle with 3 vertices at a time.

		for (int i = 0; i < 36; i += 3) {
			// get the three vertices for the triangle based
			// on the index values set above
			// use const references so we don't copy the object
			// (a good rule of thumb is to never allocate/deallocate
			// memory during *every* render/update call. This should 
			// only happen sporadically)
			const btVector3& vert1 = vertices[indices[i]];
			const btVector3& vert2 = vertices[indices[i + 1]];
			const btVector3& vert3 = vertices[indices[i + 2]];

			// create a normal that is perpendicular to the
			// face (use the cross product)
			btVector3 normal = (vert3 - vert1).cross(vert2 - vert1);
			normal.normalize();

			// set the normal for the subsequent vertices
			glNormal3f(normal.getX(), normal.getY(), normal.getZ());

			// create the vertices
			glVertex3f(vert1.x(), vert1.y(), vert1.z());
			glVertex3f(vert2.x(), vert2.y(), vert2.z());
			glVertex3f(vert3.x(), vert3.y(), vert3.z());
		}

		// stop processing vertices
		glEnd();
	}
}
