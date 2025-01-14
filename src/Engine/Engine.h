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
	private:
		//Global Pipelines
		RenderPipeline* mRenderPipeline;
		physics::PhysicsPipeline* mPhysicsPipeline;
		gbe::gui::gbuiPipeline* mGUIPipeline;
		InputSystem* mInputSystem;

		//Current Root and its handlers
		Root* current_root;

		PhysicsHandler* mPhysicsHandler;
		InputHandler* mInputHandler;
		ObjectHandler<gbe::LightObject>* mLightHandler;
		ObjectHandler<EarlyUpdate>* mEarlyUpdate;
		ObjectHandler<Update>* mUpdate;
		ObjectHandler<LateUpdate>* mLateUpdate;
	public:
		Engine();
		bool ChangeRoot(Root* newroot);
		Root* CreateBlankRoot();
		void Run();
	};
}