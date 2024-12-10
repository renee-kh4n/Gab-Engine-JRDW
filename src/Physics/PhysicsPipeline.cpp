#include "PhysicsPipeline.h"

bool gbe::physics::PhysicsPipeline::Init()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	this->overlappingPairCache = new btDbvtBroadphase();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	this->solver = new btSequentialImpulseConstraintSolver;
	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	this->dynamicsWorld->setGravity(btVector3(0, -10, 0));

	return true;
}

void gbe::physics::PhysicsPipeline::Tick(double delta)
{
	dynamicsWorld->stepSimulation(delta, 20);
}

void gbe::physics::PhysicsPipeline::RegisterBody(gbe::physics::Rigidbody* body)
{
	this->dynamicsWorld->addRigidBody(body->GetRegistrant());
}