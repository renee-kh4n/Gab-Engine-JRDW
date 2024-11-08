#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "../Graphics/RenderPipeline.h"
#include "Input/KeyDefines.h"
#include "Input/Implementations/Implementations.h"
#include "Input/InputSystem.h"
#include "ObjectHandlers/ObjectHandlers.h"
#include "Objects/Objects.h"
#include "ObjectFunctions/ObjectFunctions.h"
#include "Global/Time.h"
		  
//#include "Objects/Controllers/FlyingCameraControl.h"

namespace gde {
	class Editor {
	public:
		Editor();
		void Run();
	};
}