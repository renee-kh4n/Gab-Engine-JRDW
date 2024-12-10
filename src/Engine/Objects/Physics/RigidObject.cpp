#include "RigidObject.h"

gbe::RigidObject::RigidObject() {
	
}

gbe::RigidObject::~RigidObject()
{
}

void gbe::RigidObject::OnChangeMatrix()
{
	this->body.PassCurrentTransformMatrix(this->GetWorldMatrix());
}

void gbe::RigidObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
}
