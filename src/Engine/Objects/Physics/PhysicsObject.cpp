#include "PhysicsObject.h"

gbe::PhysicsObject::~PhysicsObject()
{
}

void gbe::PhysicsObject::OnLocalTransformationChange(TransformChangeType type)
{
	Object::OnLocalTransformationChange(type);
	this->body->InjectCurrentTransformMatrix(this->GetWorldMatrix(false));
}

void gbe::PhysicsObject::OnExternalTransformationChange(TransformChangeType type, Matrix4 parentmat)
{
	Object::OnExternalTransformationChange(type, parentmat);
	this->body->InjectCurrentTransformMatrix(this->GetWorldMatrix(false));
}

void gbe::PhysicsObject::UpdateCollider(Collider* what)
{
	this->body->UpdateColliderTransform(what->GetColliderData());
	this->body->UpdateAABB();
}

void gbe::PhysicsObject::Set_lookup_func(std::function<PhysicsObject* (physics::PhysicsBody*)>* newfunc)
{
	this->lookup_func = newfunc;
}

gbe::physics::PhysicsBody* gbe::PhysicsObject::Get_data()
{
	return this->body;
}

const std::list<gbe::Collider*> gbe::PhysicsObject::Get_colliders() {
	return this->colliders;
}

void gbe::PhysicsObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
	this->body->AddCollider(col->GetColliderData());
	col->AssignToBody(this);
}

void gbe::PhysicsObject::OnExitHierarchy(Object* newChild)
{
	Object::OnExitHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	col->UnAssignBody();
	this->body->RemoveCollider(col->GetColliderData());
	this->colliders.remove(col);
}
