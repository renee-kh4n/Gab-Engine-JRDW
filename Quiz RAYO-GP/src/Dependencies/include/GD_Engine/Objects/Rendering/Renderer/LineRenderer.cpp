#include "LineRenderer.h"

gde::LineRenderer::LineRenderer(Object* a, Object* b)
{
	this->a = a;
	this->b = b;
}

gde::Vector3 gde::LineRenderer::GetPos(int which)
{
	Object* obj = nullptr;

	if (which == 0)
		obj = this->a;
	else
		obj = this->b;

	return obj->World()->position;
}
