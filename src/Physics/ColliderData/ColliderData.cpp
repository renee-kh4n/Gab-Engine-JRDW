#include "ColliderData.h"

#include "../Rigidbody.h"

gbe::Matrix4& gbe::physics::ColliderData::GetLocalMatrix() {
	return this->local;
}

btTransform& gbe::physics::ColliderData::GetInternalTransform() {
	return this->transform;
}

void gbe::physics::ColliderData::UpdateLocalTransformation(Matrix4 mat) {
	this->local = mat;
	this->transform.setFromOpenGLMatrix(mat.Get_Ptr());

	this->owner->UpdateColliderTransform(this);
}

void gbe::physics::ColliderData::UpdateScale(PhysicsVector3 vec) {
	this->scale = vec;
	this->GetShape()->setLocalScaling(vec);

	this->owner->UpdateAABB();
}

void gbe::physics::ColliderData::AssignOwner(Rigidbody* owner) {
	this->owner = owner;
}

void gbe::physics::ColliderData::RemoveOwner() {
	this->owner = nullptr;
}