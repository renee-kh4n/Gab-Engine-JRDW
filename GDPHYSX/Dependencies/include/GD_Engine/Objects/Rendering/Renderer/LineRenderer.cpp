#include "LineRenderer.h"

gde::LineRenderer::LineRenderer(DrawCall* drawcall, Object* Camera, Object* a, Object* b) : RenderObject(drawcall)
{
	this->a = a;
	this->b = b;
	this->camera = Camera;
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

void gde::LineRenderer::InvokeEarlyUpdate()
{
	auto v1 = this->GetPos(0);
	auto v2 = this->GetPos(1);
	auto delta = v2 - v1;

	if (delta.SqrMagnitude() > 0.0f) {
		auto delta_mag = delta.Magnitude();
		auto delta_dir = delta * (1.0f / delta_mag);

		auto new_pos = (v1 + v2) * 0.5f;

		this->SetPosition(new_pos);

		auto to_eye = (this->camera->World()->position - new_pos).Normalize();
		this->SetScale(Vector3(0.7f, 1, delta_mag / 2));
		this->Orient(delta_dir, to_eye);
	}
	
	RenderObject::InvokeEarlyUpdate();
}
