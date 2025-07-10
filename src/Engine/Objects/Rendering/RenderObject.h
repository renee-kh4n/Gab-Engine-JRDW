#pragma once

#include "../Object.h"
#include "Engine/ObjectFunctions/EarlyUpdate.h"

#include "Graphics/Data/Drawcall.h"

namespace gbe {
	class RenderObject : public Object, public EarlyUpdate {
	private:
		//Rendering stuff
		gfx::DrawCall* mDrawCall;
		Matrix4* to_update;
	public:
		RenderObject(gfx::DrawCall* mDrawCall);
		virtual ~RenderObject();

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;

		inline gfx::DrawCall* Get_DrawCall() {
			return this->mDrawCall;
		}
	};
}