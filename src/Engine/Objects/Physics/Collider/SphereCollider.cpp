#include "SphereCollider.h"

gbe::SphereCollider::SphereCollider() {
	this->mData = new physics::SphereColliderData(this);
}

gbe::physics::ColliderData* gbe::SphereCollider::GetColliderData()
{
	return this->mData;
}
