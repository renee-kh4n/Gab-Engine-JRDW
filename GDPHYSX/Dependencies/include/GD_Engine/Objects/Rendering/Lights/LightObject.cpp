#include "LightObject.h"

using namespace gde;

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

bool gde::LightObject::CheckChanged()
{
	if (this->changed) {
		this->changed = false;
		return true;
	}

	auto worldmatrix = this->World();

	if ((this->old_position - worldmatrix->position).SqrMagnitude() > 0.01) {
		this->old_position = worldmatrix->position;
		return true;
	}
	if ((this->old_forward - worldmatrix->Forward).SqrMagnitude() > 0.01) {
		this->old_forward = worldmatrix->Forward;
		return true;
	}

	return false;
}
