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

	auto worldmatrix = this->World();

	Vector3 delta_pos = (this->old_position - worldmatrix->position);
	Vector3 delta_dir = (this->old_forward - worldmatrix->Forward);
	if (delta_pos.SqrMagnitude() > 0.01) {
		this->old_position = worldmatrix->position;
		return true;
	}
	if (delta_dir.SqrMagnitude() > 0.01) {
		this->old_forward = worldmatrix->Forward;
		return true;
	}

	return false;
}
