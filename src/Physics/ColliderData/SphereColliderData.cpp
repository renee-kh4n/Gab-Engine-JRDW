#include "SphereColliderData.h"

gbe::physics::SphereColliderData::SphereColliderData(Collider* related_engine_wrapper) : ColliderData(related_engine_wrapper), mShape(1)
{

}

btCollisionShape* gbe::physics::SphereColliderData::GetShape() {
	return &(this->mShape);
}