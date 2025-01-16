#include "PhysicsPipeline.h"

gbe::physics::PhysicsPipeline* gbe::physics::PhysicsPipeline::Instance;

gbe::physics::PhysicsPipeline* gbe::physics::PhysicsPipeline::Get_Instance() {
	return gbe::physics::PhysicsPipeline::Instance;
}

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
	this->dynamicsWorld->setGravity(btVector3(0, 0, 0));

	this->Instance = this;

	return true;
}

void gbe::physics::PhysicsPipeline::Tick(double delta)
{
	dynamicsWorld->stepSimulation(delta, 30);
}

void gbe::physics::PhysicsPipeline::RegisterBody(gbe::physics::PhysicsBody* body)
{
	auto registrant = body->GetRegistrant(this->dynamicsWorld);
	auto rigidbody_cast = btRigidBody::upcast(registrant);

	if (rigidbody_cast != nullptr)
		this->dynamicsWorld->addRigidBody(rigidbody_cast);
}

void gbe::physics::PhysicsPipeline::UnRegisterBody(PhysicsBody* body)
{
	auto registrant = body->UnRegister();
	auto rigidbody_cast = btRigidBody::upcast(registrant);

	if (rigidbody_cast != nullptr)
		this->dynamicsWorld->removeRigidBody(rigidbody_cast);
}