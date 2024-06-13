#pragma once

#include <list>
#include "Objects/Physics/Physics.h"

namespace gde {
	class PhysicsPipeline {
	private:
		std::list<RigidObject*> registered_objects;
	public:
		void Register(RigidObject* object);
		void Update(double duration);
	};
}