#pragma once

#include "../Object.h"

#include <GD_Graphics/DrawCall.h>
#include <GD_Graphics/RenderPipeline.h>

namespace gde {
	class RenderObject : public Object {
	private:
		//Rendering stuff
		DrawCall* mDrawCall;
	public:
		RenderObject(DrawCall* mDrawCall);
		virtual ~RenderObject() override;
	};
}