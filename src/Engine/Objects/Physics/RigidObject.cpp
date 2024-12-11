#include "RigidObject.h"

gbe::RigidObject::RigidObject(bool is_static) : body(is_static) {
	
}

gbe::RigidObject::~RigidObject()
{
}

void gbe::RigidObject::OnChangeMatrix()
{
	this->body.InjectCurrentTransformMatrix(this->GetWorldMatrix(false));
}

void gbe::RigidObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
	this->body.AddCollider(col->GetColliderData());
}

void gbe::RigidObject::OnExitHierarchy(Object* newChild)
{
	Object::OnExitHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.remove(col);
	this->body.RemoveCollider(col->GetColliderData());
}
