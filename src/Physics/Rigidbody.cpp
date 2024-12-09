#include "Rigidbody.h"

gbe::physics::Rigidbody::Rigidbody() : btRigidBody(1.0, nullptr, nullptr)
{
}

void gbe::physics::Rigidbody::PassCurrentTransformMatrix(Matrix4 pos)
{

}