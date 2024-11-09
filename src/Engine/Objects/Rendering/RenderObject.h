#pragma once

#include "../Object.h"
#include "Engine/ObjectFunctions/EarlyUpdate.h"

#include "Graphics/Drawcall.h"

namespace gbe {
	class RenderObject : public Object, public EarlyUpdate {
	private:
		//Rendering stuff
		rendering::DrawCall* mDrawCall;
		Matrix4* to_update;
	public:
		RenderObject(rendering::DrawCall* mDrawCall);
		virtual ~RenderObject();

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;
	};
}