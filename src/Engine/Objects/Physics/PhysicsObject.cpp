#include "PhysicsObject.h"

gbe::PhysicsObject::~PhysicsObject()
{
}

void gbe::PhysicsObject::OnLocalTransformationChange(TransformChangeType type)
{
	Object::OnLocalTransformationChange(type);
	this->UpdatePhysicsTransformationMatrix();
}

void gbe::PhysicsObject::OnExternalTransformationChange(TransformChangeType type, Matrix4 parentmat)
{
	Object::OnExternalTransformationChange(type, parentmat);


	this->UpdatePhysicsTransformationMatrix();
}

void gbe::PhysicsObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
	this->OnAddCollider(col);
}

void gbe::PhysicsObject::OnExitHierarchy(Object* newChild)
{
	Object::OnExitHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	col->UnAssignRigidbody();
	this->colliders.remove(col);
	this->OnRemoveCollider(col);
}
