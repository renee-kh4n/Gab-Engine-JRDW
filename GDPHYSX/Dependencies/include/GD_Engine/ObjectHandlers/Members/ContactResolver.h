#pragma once

#include "../../Objects/Physics/Physics.h"

namespace gde {
	class ContactResolver {
	public:
		unsigned max_iterations;
		ContactResolver(unsigned _max_iterations) : max_iterations(_max_iterations) {}

		void ResolveContacts(std::vector<CollisionContact*> contacts, float time);
	};
}