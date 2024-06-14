#pragma once

#include <list>
#include "ObjectHandler.h"
#include "../Objects/Physics/Physics.h"
#include "../Objects/Physics/ForceVolume.h"

namespace gde {
	class PhysicsHandler : public ObjectHandler<RigidObject>{
	private:
		ObjectHandler<ForceVolume> forcevolume_handler;
	public:
		PhysicsHandler();

		void Update(double duration);
	};
}