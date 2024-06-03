#pragma once

#include <list>
#include "Object.h"

namespace gde {
	class PhysicsPipeline {
	private:
		std::list<Object*> registered_objects;
	public:
		void Register(Object* object);
		void Update(double duration);
	};
}