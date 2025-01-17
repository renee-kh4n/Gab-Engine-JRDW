#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "Graphics/gbe_graphics.h"
#include "GUI/gbe_gui.h"
#include "Math/gbe_math.h"
#include "Physics/gbe_physics.h"
#include "Window/gbe_window.h"
#include "Asset/gbe_asset.h"
#include "Objects/Root.h"
#include "ObjectHandlers/PhysicsHandler.h"
#include "ObjectHandlers/ObjectHandler.h"
#include "ObjectFunctions/EarlyUpdate.h"
#include "ObjectFunctions/LateUpdate.h"
#include "ObjectFunctions/Update.h"
#include "ObjectFunctions/PhysicsUpdate.h"
#include "Objects/Rendering/Lights/LightObject.h"
#include "Objects/Rendering/Camera.h"

namespace gbe {
	class Engine {
	private:
		//Current Root and its handlers
		Root* current_root;
		Root* queued_rootchange;
	public:
		Engine();
		bool ChangeRoot(Root* newroot);
		Root* CreateBlankRoot();
		void Run();
	};
}