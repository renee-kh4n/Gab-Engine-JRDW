#include "BoxColliderData.h"

gbe::physics::BoxColliderData::BoxColliderData() : mShape((PhysicsVector3)Vector3(1, 1, 1))
{

}

btCollisionShape* gbe::physics::BoxColliderData::GetShape()
{
	return &(this->mShape);
}
