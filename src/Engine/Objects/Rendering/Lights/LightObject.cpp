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

void gbe::LightObject::OnLocalTransformationChange(TransformChangeType type)
{
	Object::OnLocalTransformationChange(type);

	this->changed = true;
}

void gbe::LightObject::OnExternalTransformationChange(TransformChangeType type, Matrix4 parentmat)
{
	Object::OnExternalTransformationChange(type, parentmat);

	this->changed = true;
}
