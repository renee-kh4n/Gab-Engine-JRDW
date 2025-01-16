#include "RigidObject.h"
#include "RigidObject.h"
#include "RigidObject.h"
#include "RigidObject.h"

gbe::RigidObject::RigidObject(bool is_static) : body(is_static) {
	
}

void gbe::RigidObject::OnAddCollider(Collider* what)
{
	this->body.AddCollider(what->GetColliderData());
	what->AssignToRigidbody(this);
}

void gbe::RigidObject::OnRemoveCollider(Collider* what)
{
	this->body.RemoveCollider(what->GetColliderData());
	what->UnAssignRigidbody();
}

gbe::RigidObject::~RigidObject()
{
}

void gbe::RigidObject::UpdateCollider(Collider* what)
{
	this->body.UpdateColliderTransform(what->GetColliderData());
	this->body.UpdateAABB();
}

void gbe::RigidObject::UpdatePhysicsTransformationMatrix()
{
	this->body.InjectCurrentTransformMatrix(this->GetWorldMatrix(false));
}
