#pragma once

#include <vector>
#include "Object.h"

namespace gde {
	class PhysicsPipeline {
	private:
		std::vector<Object*> registered_objects;
	public:
		void Register(Object* object);
		void Update(double duration);
	};
}