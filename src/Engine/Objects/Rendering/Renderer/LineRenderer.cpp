#include "LineRenderer.h"

using namespace gbe::rendering;

gbe::LineRenderer::LineRenderer(DrawCall* drawcall, Object* Camera, Object* a, Object* b) : RenderObject(drawcall)
{
	this->a = a;
	this->b = b;
	this->camera = Camera;
}

gbe::Vector3 gbe::LineRenderer::GetPos(int which)
{
	Object* obj = nullptr;

	if (which == 0)
		obj = this->a;
	else
		obj = this->b;

	return obj->World()->position;
}

void gbe::LineRenderer::InvokeEarlyUpdate()
{
	auto v1 = this->GetPos(0);
	auto v2 = this->GetPos(1);
	Vector3 delta = v2 - v1;

	if (delta.SqrMagnitude() > 0.0f) {
		auto delta_mag = delta.Magnitude();
		auto delta_dir = delta * (1.0f / delta_mag);

		Vector3 new_pos = (v1 + v2) * 0.5f;

		this->SetPosition(new_pos);

		Vector3 delta_pos = (this->camera->World()->position - new_pos);
		Vector3 to_eye = delta_pos.Normalize();
		this->SetScale(Vector3(0.1f, 1, delta_mag / 2));
		this->Orient(delta_dir, to_eye);
	}
	
	RenderObject::InvokeEarlyUpdate();
}
