#include "BoxColliderData.h"

gbe::physics::BoxColliderData::BoxColliderData(Collider* related_engine_wrapper) : ColliderData(related_engine_wrapper), mShape((PhysicsVector3)Vector3(1, 1, 1))
{
}

btCollisionShape* gbe::physics::BoxColliderData::GetShape()
{
	return &(this->mShape);
}
