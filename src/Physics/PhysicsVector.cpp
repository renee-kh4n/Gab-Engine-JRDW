#include "PhysicsVector.h"

gbe::physics::PhysicsVector3::PhysicsVector3() : gbe::Vector3()
{

}

gbe::physics::PhysicsVector3::PhysicsVector3(const Vector3& normalvec) : gbe::Vector3(normalvec)
{

}

gbe::physics::PhysicsVector3::PhysicsVector3(const btVector3& btvec)
{
	this->x = btvec.x();
	this->y = btvec.y();
	this->z = btvec.z();
}

gbe::physics::PhysicsVector3::operator btVector3() const
{
	return btVector3(this->x, this->y, this->z);
}