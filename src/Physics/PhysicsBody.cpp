#include "PhysicsBody.h"

gbe::physics::PhysicsBody::PhysicsBody()
{
	this->base_data = nullptr;
	this->world = nullptr;

	this->transform.setIdentity();

	this->mMainShape = new btCompoundShape();

	this->motionstate = new btDefaultMotionState(this->transform);
}

void gbe::physics::PhysicsBody::InjectCurrentTransformMatrix(Matrix4 pos)
{
	this->transform.setFromOpenGLMatrix(pos.Get_Ptr());
	this->base_data->setWorldTransform(this->transform);
	this->motionstate->setWorldTransform(this->transform);
}

void gbe::physics::PhysicsBody::PassTransformationData(Vector3& pos, Quaternion& rot)
{
	this->ForceUpdateTransform();
	pos = (PhysicsVector3)this->transform.getOrigin();
	rot = (PhysicsQuaternion)this->transform.getRotation();
}

void gbe::physics::PhysicsBody::PassTransformationMatrix(Matrix4& mat)
{
	this->ForceUpdateTransform();
	this->transform.getOpenGLMatrix((float*)mat.Get_Ptr());
}

btCollisionObject* gbe::physics::PhysicsBody::GetRegistrant(btDynamicsWorld* register_to)
{
	this->world = register_to;
	return this->base_data;
}

btCollisionObject* gbe::physics::PhysicsBody::UnRegister()
{
	this->world = nullptr;
	return this->base_data;
}

void gbe::physics::PhysicsBody::AddCollider(ColliderData* col)
{
	this->mMainShape->addChildShape(col->GetInternalTransform(), col->GetShape());
}

void gbe::physics::PhysicsBody::UpdateColliderTransform(ColliderData* col)
{

	int index = 0;
	for (size_t i = 0; i < this->mMainShape->getNumChildShapes(); i++)
	{
		if (this->mMainShape->getChildShape(i) == col->GetShape()) {
			index = i;
			break;
		}
	}

	this->mMainShape->updateChildTransform(index, col->GetInternalTransform());
}

void gbe::physics::PhysicsBody::RemoveCollider(ColliderData* col)
{
	this->mMainShape->removeChildShape(col->GetShape());
}

void gbe::physics::PhysicsBody::UpdateAABB()
{
	if (this->world == nullptr)
		return;

	this->world->updateSingleAabb(this->base_data);
}


