#pragma once

#include "../Object.h"
#include "../../ObjectFunctions/EarlyUpdate.h"

#include <GD_Graphics/DrawCall.h>
#include <GD_Graphics/RenderPipeline.h>

namespace gde {
	class RenderObject : public Object, public EarlyUpdate {
	private:
		//Rendering stuff
		rendering::DrawCall* mDrawCall;
	public:
		RenderObject(rendering::DrawCall* mDrawCall);
		virtual ~RenderObject();

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;
	};
}