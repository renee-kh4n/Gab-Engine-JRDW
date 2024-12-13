#include "RigidObject.h"

gbe::RigidObject::RigidObject(bool is_static) : body(is_static) {
	
}

gbe::RigidObject::~RigidObject()
{
}

void gbe::RigidObject::OnLocalTransformationChange(TransformChangeType type)
{
	Object::OnLocalTransformationChange(type);

	this->body.InjectCurrentTransformMatrix(this->GetWorldMatrix(false));
}

void gbe::RigidObject::OnExternalTransformationChange(TransformChangeType type, Matrix4 parentmat)
{
	Object::OnExternalTransformationChange(type, parentmat);

	this->body.InjectCurrentTransformMatrix(this->GetWorldMatrix(false));
}

void gbe::RigidObject::UpdateCollider(Collider* what)
{
	this->body.UpdateColliderTransform(what->GetColliderData());
	this->body.UpdateAABB();
}

void gbe::RigidObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
	this->body.AddCollider(col->GetColliderData());
	col->AssignToRigidbody(this);
}

void gbe::RigidObject::OnExitHierarchy(Object* newChild)
{
	Object::OnExitHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	col->UnAssignRigidbody();
	this->colliders.remove(col);
	this->body.RemoveCollider(col->GetColliderData());
}
