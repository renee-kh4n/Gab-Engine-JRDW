#include "BoxCollider.h"

gbe::BoxCollider::BoxCollider() {
	this->mData = new physics::BoxColliderData();
}

gbe::physics::ColliderData* gbe::BoxCollider::GetColliderData()
{
	return this->mData;
}
