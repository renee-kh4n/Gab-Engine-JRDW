#pragma once

#include <list>
#include "ObjectHandler.h"
#include "../Objects/Rendering/Renderer/LineRenderer.h"

namespace gde {

	class LineHandler : public ObjectHandler<LineRenderer> {
	private:
		
	public:
		LineHandler();

		void DrawAll();
	};
}