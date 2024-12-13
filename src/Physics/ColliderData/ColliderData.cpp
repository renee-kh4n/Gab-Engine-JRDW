#include "ColliderData.h"

#include "../Rigidbody.h"

gbe::physics::ColliderData::ColliderData() {
	
}

gbe::Matrix4& gbe::physics::ColliderData::GetLocalMatrix() {
	return this->local;
}

btTransform& gbe::physics::ColliderData::GetInternalTransform() {
	return this->transform;
}

void gbe::physics::ColliderData::UpdateLocalTransformation(Matrix4 mat) {
	this->local = mat;
	this->transform.setFromOpenGLMatrix(mat.Get_Ptr());
	//this->transform.setOrigin((PhysicsVector3)Vector3(0, 0, 0));
}

void gbe::physics::ColliderData::UpdateScale(PhysicsVector3 vec) {
	this->scale = vec;
	this->GetShape()->setLocalScaling(vec);
}