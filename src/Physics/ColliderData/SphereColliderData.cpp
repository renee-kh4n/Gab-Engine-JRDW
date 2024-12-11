#include "SphereColliderData.h"

gbe::physics::SphereColliderData::SphereColliderData() : mShape(1)
{

}

btCollisionShape* gbe::physics::SphereColliderData::GetShape() {
	return &(this->mShape);
}