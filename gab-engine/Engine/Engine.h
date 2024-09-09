#pragma once

#include "Input/KeyDefines.h"

#include "Graphics/RenderPipeline.h"

#include "Input/Implementations/Implementations.h"
#include "Input/InputSystem.h"
#include "ObjectHandlers/ObjectHandlers.h"
#include "Objects/Objects.h"
#include "ObjectFunctions/ObjectFunctions.h"
#include "Datatypes/Vectors.h"
#include "Global/Time.h"
		  
#include "Objects/Physics/Joints/Spinner.h"
		  
#include "Objects/Controllers/FlyingCameraControl.h"

namespace gde {
	class Engine {
	public:
		Engine();
		void Run();
	};
}