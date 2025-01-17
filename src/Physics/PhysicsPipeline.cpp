#include "PhysicsPipeline.h"
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

gbe::physics::PhysicsPipeline* gbe::physics::PhysicsPipeline::Instance;

void gbe::physics::PhysicsPipeline::internal_physics_callback(btDynamicsWorld* world, btScalar timeStep)
{
}

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
	this->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	
	auto callback = [](btDynamicsWorld* world, btScalar timeStep) {
		physics::PhysicsPipeline::Get_Instance()->OnFixedUpdate_callback(timeStep);
	};
	this->dynamicsWorld->setInternalTickCallback(callback, this, false);

	this->Instance = this;

	return true;
}

void gbe::physics::PhysicsPipeline::Tick(double delta)
{
	dynamicsWorld->stepSimulation(delta, 30);
}

void gbe::physics::PhysicsPipeline::RegisterBody(gbe::physics::PhysicsBody* body)
{
	data_wrapper_dictionary.insert_or_assign(body->Get_wrapped_data(), body);
	body->Register(this->dynamicsWorld);
}

void gbe::physics::PhysicsPipeline::UnRegisterBody(PhysicsBody* body)
{
	data_wrapper_dictionary.erase(body->Get_wrapped_data());
	body->UnRegister();
}

void gbe::physics::PhysicsPipeline::Set_OnFixedUpdate_callback(std::function<void(float physicsdeltatime)> newfunc)
{
	this->OnFixedUpdate_callback = newfunc;
}

gbe::physics::PhysicsBody* gbe::physics::PhysicsPipeline::GetRelatedBody(btCollisionObject* key) {
	auto enume = this->data_wrapper_dictionary.find(key);

	if (enume == this->data_wrapper_dictionary.end())
		return nullptr;

	return enume->second;
}