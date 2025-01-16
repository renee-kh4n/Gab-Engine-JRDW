#include "Collider.h"
#include "Engine/Objects/Physics/RigidObject.h"

gbe::PhysicsObject* gbe::Collider::GetBody()
{
	return this->holder;
}

void gbe::Collider::AssignToBody(PhysicsObject* body)
{
	this->holder = body;
}

void gbe::Collider::UnAssignBody()
{
	this->holder = nullptr;
}

void gbe::Collider::OnLocalTransformationChange(TransformChangeType type)
{
	Object::OnLocalTransformationChange(type);

	this->GetColliderData()->UpdateLocalTransformation(this->Local().GetMatrix());

	if (type & TransformChangeType::SCALE)
		this->GetColliderData()->UpdateScale(this->World().scale.Get());

	if (this->holder == nullptr)
		return;

	this->holder->UpdateCollider(this);
}

void gbe::Collider::OnExternalTransformationChange(TransformChangeType type, Matrix4 newparentmat)
{
	Object::OnExternalTransformationChange(type, newparentmat);

	if (type & TransformChangeType::SCALE) {
		this->GetColliderData()->UpdateScale(this->World().scale.Get());

		if (this->holder == nullptr)
			return;

		this->holder->UpdateCollider(this);
	}
}
