#pragma once

#include "../Object.h"
#include "../../UpdateType/EarlyUpdate.h"

namespace gde {
	class RenderObject : public Object, public EarlyUpdate {
	private:
		//Rendering stuff
		DrawCall* mDrawCall;
	public:
		RenderObject(DrawCall* mDrawCall);
		virtual ~RenderObject() override;

		// Inherited via EarlyUpdate
		virtual void InvokeEarlyUpdate() override;
	};
}