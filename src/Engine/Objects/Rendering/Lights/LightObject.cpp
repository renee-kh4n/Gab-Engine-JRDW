#include "LightObject.h"

using namespace gbe;

void LightObject::Set_Color(Vector3 color) {
	this->Color = color;
	this->changed = true;
}
float LightObject::Get_Intensity() {
	return this->intensity;
}
void LightObject::Set_Intensity(float intensity) {
	this->intensity = intensity;
	this->changed = true;
}

bool gbe::LightObject::CheckChanged()
{
	if (this->changed) {
		this->changed = false;
		return true;
	}

	auto worldTransform = this->World();

	Vector3 delta_pos = (this->old_position - worldTransform.position.Get());
	Vector3 delta_dir = (this->old_forward - worldTransform.GetForward());
	if (delta_pos.SqrMagnitude() > 0.01) {
		this->old_position = worldTransform.position.Get();
		return true;
	}
	if (delta_dir.SqrMagnitude() > 0.01) {
		this->old_forward = worldTransform.GetForward();
		return true;
	}

	return false;
}
