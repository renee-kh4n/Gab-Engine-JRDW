#pragma once

#include "../../Object.h"
#include "../../../ObjectFunctions/EarlyUpdate.h"

namespace gde {
	class LineRenderer : public Object{
	private:
		Object* a;
		Object* b;
	
	public:
		LineRenderer(Object* a, Object* b);

		Vector3 GetPos(int which);
		//virtual void InvokeEarlyUpdate() override;
	};
}