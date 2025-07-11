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
		const bool editor = false;
	public:
		RenderObject(gfx::DrawCall* mDrawCall, bool editor = false);
		virtual ~RenderObject();

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;

		inline gfx::DrawCall* Get_DrawCall() {
			return this->mDrawCall;
		}

		bool is_editor() {
			return this->editor;
		}
	};
}