#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "Graphics/gbe_graphics.h"
#include "GUI/gbe_gui.h"
#include "Math/gbe_math.h"
#include "Physics/gbe_physics.h"
#include "Asset/gbe_asset.h"
#include "gbe_engine.h"

namespace gbe {
	class Engine {
	public:
		Engine();
		void Run();
	};
}