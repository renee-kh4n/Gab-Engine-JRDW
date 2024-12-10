#include "Rigidbody.h"

#include <glm/gtc/type_ptr.hpp>

gbe::physics::Rigidbody::Rigidbody()
{
	this->transform.setIdentity();

	btScalar mass(1.0f);
	btVector3 localInertia(0, 0, 0);

	this->motionstate = new btDefaultMotionState(this->transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, this->motionstate, new btSphereShape(1.0f), localInertia);
	this->data = new btRigidBody(rbInfo);
}

void gbe::physics::Rigidbody::PassCurrentTransformMatrix(Matrix4 pos)
{
	this->transform.setFromOpenGLMatrix(pos.Get_Ptr());
	this->data->setWorldTransform(this->transform);
	this->motionstate->setWorldTransform(this->transform);
}

void gbe::physics::Rigidbody::GetCalculatedMatrix(Matrix4* target)
{
	this->data->getMotionState()->getWorldTransform(this->transform);
	this->transform.getOpenGLMatrix((float*)target);
}

btRigidBody* gbe::physics::Rigidbody::GetRegistrant()
{
	return this->data;
}

float gbe::physics::Rigidbody::Get_mass()
{
	return 1.0f;
}

void gbe::physics::Rigidbody::AddForce(PhysicsVector3 force)
{

}

void gbe::physics::Rigidbody::AddForceAtPoint(PhysicsVector3 force, PhysicsVector3 relativeWorldPoint)
{

}

void gbe::physics::Rigidbody::Set_velocity(PhysicsVector3 value)
{
	this->data->setLinearVelocity(value);
}

gbe::physics::PhysicsVector3 gbe::physics::Rigidbody::Get_velocity()
{
	return this->data->getLinearVelocity();
}

void gbe::physics::Rigidbody::Set_angularVelocity(PhysicsVector3 value)
{
	this->data->setAngularVelocity(value);
}

gbe::physics::PhysicsVector3 gbe::physics::Rigidbody::Get_angularVelocity()
{
	return PhysicsVector3();
}