#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "../Graphics/RenderPipeline.h"

#include "../GUI/gbuiPipeline.h"
#include "../GUI/rect_types/gb_button.h"

#include "Input/KeyDefines.h"
#include "Input/Implementations/Implementations.h"
#include "Input/InputSystem.h"
		  
#include "ObjectHandlers/ObjectHandlers.h"

#include "Objects/Objects.h"
#include "ObjectFunctions/ObjectFunctions.h"
#include "Objects/Controllers/FlyingCameraControl.h"
#include "Objects/Controllers/GenericController.h"
#include "Objects/Physics/Joints/Spinner.h"
#include "Objects/Physics/Collider/SphereCollider.h"
#include "Objects/Physics/Collider/BoxCollider.h"

#include "../Physics/PhysicsDatatypes.h"

#include "Global/Time.h"

namespace gbe {
	class Engine {
	public:
		Engine();
		void Run();
	};
}