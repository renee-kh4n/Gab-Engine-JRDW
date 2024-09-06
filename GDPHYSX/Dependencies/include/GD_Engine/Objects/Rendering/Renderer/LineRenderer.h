#pragma once

#include "../RenderObject.h"
#include "../../../ObjectFunctions/EarlyUpdate.h"

namespace gde {
	class LineRenderer : public RenderObject{
	private:
		Object* a;
		Object* b;
		Object* camera;
	
	public:
		LineRenderer(DrawCall* drawcall, Object* Camera, Object* a, Object* b);

		Vector3 GetPos(int which);
		virtual void InvokeEarlyUpdate();
	};
}