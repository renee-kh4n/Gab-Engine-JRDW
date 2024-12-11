#pragma once

#include "Engine/Objects/Rendering/RenderObject.h"
#include "Engine/ObjectFunctions/EarlyUpdate.h"

namespace gbe {
	class LineRenderer : public RenderObject{
	private:
		Object* a;
		Object* b;
		Object* camera;
		float thickness = 0.1f;
	public:
		LineRenderer(rendering::DrawCall* drawcall, Object* Camera, Object* a, Object* b);

		void SetThickness(float value);
		Vector3 GetPos(int which);
		virtual void InvokeEarlyUpdate();
	};
}