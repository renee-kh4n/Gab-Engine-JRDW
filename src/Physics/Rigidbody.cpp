#include "Rigidbody.h"

#include <glm/gtc/type_ptr.hpp>

gbe::physics::Rigidbody::Rigidbody()
{
	this->transform.setIdentity();

	btScalar mass(1.0f);
	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(this->transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, myMotionState, nullptr, localInertia);
	this->data = new btRigidBody(rbInfo);
}

void gbe::physics::Rigidbody::PassCurrentTransformMatrix(Matrix4 pos)
{
	this->transform.setFromOpenGLMatrix(pos.Get_Ptr());
}

btRigidBody* gbe::physics::Rigidbody::GetRegistrant()
{
	return this->data;
}

float gbe::physics::Rigidbody::Get_mass()
{
	return 1.0f;
}

void gbe::physics::Rigidbody::AddForce(Vector3 force)
{
}

void gbe::physics::Rigidbody::AddForceAtPoint(Vector3 force, Vector3 relativeWorldPoint)
{
}

void gbe::physics::Rigidbody::Set_velocity(Vector3 value)
{
	//this->data->setLinearVelocity(value);
}

gbe::Vector3 gbe::physics::Rigidbody::Get_velocity()
{
	return Vector3();
}

void gbe::physics::Rigidbody::Set_angularVelocity(Vector3 value)
{
}

gbe::Vector3 gbe::physics::Rigidbody::Get_angularVelocity()
{
	return Vector3();
}