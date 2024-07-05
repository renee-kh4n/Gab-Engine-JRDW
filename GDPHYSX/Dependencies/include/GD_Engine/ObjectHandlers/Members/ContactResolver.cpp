#include "ContactResolver.h"

void gde::ContactResolver::ResolveContacts(std::vector<CollisionContact*> contacts, float time)
{
	for (auto& contact : contacts)
	{
		contact->Resolve(time);
	}
}
