#include "ContactResolver.h"

void gbe::ContactResolver::ResolveContacts(std::vector<CollisionContact*> contacts, float time)
{
	int iterations = 0;
	while (iterations < this->max_iterations)
	{
		for (auto& contact : contacts)
		{
			contact->Resolve(time);
		}

		iterations++;
	}
}