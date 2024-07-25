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
	auto delta_dir = delta.Normalize();
	auto delta_mag = delta.x / delta_dir.x;
	auto new_pos = (v1 + v2) * 0.5f;

	this->SetPosition(new_pos);
	this->SetScale(Vector3(1, 1, delta.Magnitude() / 2));
	//this->SetScale(Vector3(delta_mag, delta_mag, delta_mag));
	auto to_eye = (this->camera->World()->position - new_pos).Normalize();
	this->Orient(delta_dir, to_eye);
	
	RenderObject::InvokeEarlyUpdate();
}
