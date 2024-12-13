#include "Rigidbody.h"

#include <glm/gtc/type_ptr.hpp>

gbe::physics::Rigidbody::Rigidbody(bool is_static)
{
	this->transform.setIdentity();

	this->mass = 1.0f;

	btScalar mass(is_static ? 0 : this->mass);
	btVector3 localInertia(0, 0, 0);

	this->mMainShape = new btCompoundShape();

	this->motionstate = new btDefaultMotionState(this->transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, this->motionstate, this->mMainShape, localInertia);
	this->data = new btRigidBody(rbInfo);
}

void gbe::physics::Rigidbody::InjectCurrentTransformMatrix(Matrix4 pos)
{
	this->transform.setFromOpenGLMatrix(pos.Get_Ptr());
	this->data->setWorldTransform(this->transform);
	this->motionstate->setWorldTransform(this->transform);
}

void gbe::physics::Rigidbody::PassTransformationData(Vector3& pos, Quaternion& rot)
{
	this->data->getMotionState()->getWorldTransform(this->transform);
	pos = (PhysicsVector3)this->transform.getOrigin();
	rot = (PhysicsQuaternion)this->transform.getRotation();
}

void gbe::physics::Rigidbody::PassTransformationMatrix(Matrix4& mat)
{
	this->data->getMotionState()->getWorldTransform(this->transform);
	this->transform.getOpenGLMatrix((float*)mat.Get_Ptr());
}

btRigidBody* gbe::physics::Rigidbody::GetRegistrant(btDynamicsWorld* register_to)
{
	this->world = register_to;
	return this->data;
}

btRigidBody* gbe::physics::Rigidbody::UnRegister()
{
	this->world = nullptr;
	return this->data;
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
	this->data->applyCentralForce(force);
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

void gbe::physics::Rigidbody::AddCollider(ColliderData* data) {
	this->mMainShape->addChildShape(data->GetInternalTransform(), data->GetShape());
}

void gbe::physics::Rigidbody::UpdateColliderTransform(ColliderData* data) {
	
	int index = 0;
	for (size_t i = 0; i < this->mMainShape->getNumChildShapes(); i++)
	{
		if (this->mMainShape->getChildShape(i) == data->GetShape()) {
			index = i;
			break;
		}
	}

	this->mMainShape->updateChildTransform(index, data->GetInternalTransform());
}

void gbe::physics::Rigidbody::RemoveCollider(ColliderData* data) {
	this->mMainShape->removeChildShape(data->GetShape());
}

void gbe::physics::Rigidbody::UpdateAABB() {
	if (this->world == nullptr)
		return;

	this->world->updateSingleAabb(this->data);
}