#pragma once

#include "../Object.h"
#include "../../ObjectFunctions/EarlyUpdate.h"

#include "../Graphics/Drawcall.h"

namespace gde {
	class RenderObject : public Object, public EarlyUpdate {
	private:
		//Rendering stuff
		rendering::DrawCall* mDrawCall;
		glm::mat4* to_update;
	public:
		RenderObject(rendering::DrawCall* mDrawCall);
		virtual ~RenderObject();

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;
	};
}