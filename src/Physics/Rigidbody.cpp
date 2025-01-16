#include "Rigidbody.h"

#include <glm/gtc/type_ptr.hpp>

gbe::physics::Rigidbody::Rigidbody(bool is_static){
	this->mass = 1.0f;

	btScalar mass(is_static ? 0 : this->mass);
	btVector3 localInertia(0, 0, 0);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, this->motionstate, this->mMainShape, localInertia);
	auto newdata = new btRigidBody(rbInfo);
	this->data_derived = newdata;
	this->base_data = newdata;
}

void gbe::physics::Rigidbody::ForceUpdateTransform() {
	this->data_derived->getMotionState()->getWorldTransform(this->transform);
}

float gbe::physics::Rigidbody::Get_mass()
{
	return 1.0f;
}

void gbe::physics::Rigidbody::SetStatic(bool value) {
	throw "not implemented";
}

void gbe::physics::Rigidbody::AddForce(PhysicsVector3 force)
{
	this->data_derived->applyCentralForce(force);
}

void gbe::physics::Rigidbody::AddForceAtPoint(PhysicsVector3 force, PhysicsVector3 relativeWorldPoint)
{

}

void gbe::physics::Rigidbody::Set_velocity(PhysicsVector3 value)
{
	this->data_derived->setLinearVelocity(value);
}

gbe::physics::PhysicsVector3 gbe::physics::Rigidbody::Get_velocity()
{
	return this->data_derived->getLinearVelocity();
}

void gbe::physics::Rigidbody::Set_angularVelocity(PhysicsVector3 value)
{
	this->data_derived->setAngularVelocity(value);
}

gbe::physics::PhysicsVector3 gbe::physics::Rigidbody::Get_angularVelocity()
{
	return PhysicsVector3();
}

