#include "Collider.h"

void gbe::Collider::OnChangeMatrix()
{
	this->GetColliderData()->UpdateLocalTransformation(this->Local().GetMatrix());
	this->GetColliderData()->UpdateScale(this->World().scale.Get());
}
