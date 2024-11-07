#pragma once

#include "../Object.h"
#include "../../ObjectFunctions/EarlyUpdate.h"

namespace gde {
	namespace rendering {
		class DrawCall;
	}

	class RenderObject : public Object, public EarlyUpdate {
	private:
		//Rendering stuff
		rendering::DrawCall* mDrawCall;
		glm::mat4* to_update;
	public:
		RenderObject(DrawCall* mDrawCall);
		virtual ~RenderObject();

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;
	};
}