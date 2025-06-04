#include "Rigidbody.h"

#include <glm/gtc/type_ptr.hpp>

gbe::physics::Rigidbody::Rigidbody(PhysicsObject* object, bool is_static) : PhysicsBody(object){
	this->mass = 1.0f;

	btScalar mass(is_static ? 0 : this->mass);
	btVector3 localInertia = (PhysicsVector3)Vector3(1.0f);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, this->motionstate, this->mMainShape, localInertia);
	auto newdata = new btRigidBody(rbInfo);
	newdata->setFriction(0.5f);
	newdata->setRollingFriction(.1);
	newdata->setSpinningFriction(0.1);

	this->base_data = newdata;
}

void gbe::physics::Rigidbody::Register(btDynamicsWorld* register_to)
{
	register_to->addRigidBody(btRigidBody::upcast(this->base_data));
	PhysicsBody::Register(register_to);
}

void gbe::physics::Rigidbody::UnRegister()
{
	this->world->removeRigidBody(btRigidBody::upcast(this->base_data));
	PhysicsBody::UnRegister();
}

void gbe::physics::Rigidbody::ForceUpdateTransform() {
	btRigidBody::upcast(this->base_data)->getMotionState()->getWorldTransform(this->transform);
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
	btRigidBody::upcast(this->base_data)->applyCentralForce(force);
}

void gbe::physics::Rigidbody::AddContinuousForce(PhysicsVector3 force)
{
	this->continouus_forces_this_frame += force;
}

void gbe::physics::Rigidbody::AddForceAtPoint(PhysicsVector3 force, PhysicsVector3 relativeWorldPoint)
{

}

void gbe::physics::Rigidbody::Pre_Tick_function(float deltatime)
{
	this->AddForce((PhysicsVector3)(this->continouus_forces_this_frame * deltatime));
	this->continouus_forces_this_frame = Vector3::zero;
}

void gbe::physics::Rigidbody::Set_velocity(PhysicsVector3 value)
{
	btRigidBody::upcast(this->base_data)->setLinearVelocity(value);
}

gbe::physics::PhysicsVector3 gbe::physics::Rigidbody::Get_velocity()
{
	return btRigidBody::upcast(this->base_data)->getLinearVelocity();
}

void gbe::physics::Rigidbody::Set_angularVelocity(PhysicsVector3 value)
{
	btRigidBody::upcast(this->base_data)->setAngularVelocity(value);
}

gbe::physics::PhysicsVector3 gbe::physics::Rigidbody::Get_angularVelocity()
{
	return PhysicsVector3();
}

