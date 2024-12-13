#include "Collider.h"
#include "../RigidObject.h"

gbe::RigidObject* gbe::Collider::GetRigidbody()
{
	return this->rigidparent;
}

void gbe::Collider::AssignToRigidbody(RigidObject* body)
{
	this->rigidparent = body;
}

void gbe::Collider::UnAssignRigidbody()
{
	this->rigidparent = nullptr;
}

void gbe::Collider::OnLocalTransformationChange(TransformChangeType type)
{
	Object::OnLocalTransformationChange(type);

	this->GetColliderData()->UpdateLocalTransformation(this->Local().GetMatrix());

	if (type & TransformChangeType::SCALE)
		this->GetColliderData()->UpdateScale(this->World().scale.Get());

	if (this->rigidparent == nullptr)
		return;

	this->rigidparent->UpdateCollider(this);
}

void gbe::Collider::OnExternalTransformationChange(TransformChangeType type, Matrix4 newparentmat)
{
	Object::OnExternalTransformationChange(type, newparentmat);

	if (type & TransformChangeType::SCALE) {
		this->GetColliderData()->UpdateScale(this->World().scale.Get());

		if (this->rigidparent == nullptr)
			return;

		this->rigidparent->UpdateCollider(this);
	}
}
