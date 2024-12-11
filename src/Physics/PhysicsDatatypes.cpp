#include "PhysicsDatatypes.h"


//VECTOR 3
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

//QUATERNION

gbe::physics::PhysicsQuaternion::PhysicsQuaternion() : gbe::Quaternion()
{

}

gbe::physics::PhysicsQuaternion::PhysicsQuaternion(const Quaternion& normalquat) : gbe::Quaternion(normalquat)
{

}

gbe::physics::PhysicsQuaternion::PhysicsQuaternion(const btQuaternion& btquat)
{
	this->x = btquat.x();
	this->y = btquat.y();
	this->z = btquat.z();
	this->w = btquat.w();
}

gbe::physics::PhysicsQuaternion::operator btQuaternion() const
{
	return btQuaternion(this->x, this->y, this->z, this->w);
}