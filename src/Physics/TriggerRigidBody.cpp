#include "TriggerRigidBody.h"
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "PhysicsPipeline.h"

gbe::physics::TriggerRigidBody::TriggerRigidBody() {
	auto newdata = new btGhostObject();

	newdata->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	newdata->setCollisionShape(this->mMainShape);
	newdata->setWorldTransform(this->transform);

	this->base_data = newdata;
}

void gbe::physics::TriggerRigidBody::Pre_Tick_function(float deltatime)
{
}

void gbe::physics::TriggerRigidBody::Register(btDynamicsWorld* register_to)
{
	register_to->addCollisionObject(this->base_data);
	PhysicsBody::Register(register_to);
}

void gbe::physics::TriggerRigidBody::UnRegister()
{
	this->world->removeCollisionObject(this->base_data);
	PhysicsBody::UnRegister();
}

int gbe::physics::TriggerRigidBody::Get_numInside() {
	return btGhostObject::upcast(this->base_data)->getNumOverlappingObjects();
}

gbe::physics::PhysicsBody* gbe::physics::TriggerRigidBody::Get_inside(int index) {
	auto key = btGhostObject::upcast(this->base_data)->getOverlappingObject(index);
	return physics::PhysicsPipeline::Get_Instance()->GetRelatedBody(key);
}

void gbe::physics::TriggerRigidBody::ForceUpdateTransform()
{

}