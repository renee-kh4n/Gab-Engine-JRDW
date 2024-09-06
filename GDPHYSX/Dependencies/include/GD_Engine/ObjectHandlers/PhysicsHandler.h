#pragma once

#include <list>
#include "ObjectHandler.h"
#include "../Objects/Physics/Physics.h"
#include "../Objects/Physics/ForceVolume.h"
#include "Members/ContactResolver.h"

namespace gde {
	class PhysicsHandler : public ObjectHandler<RigidObject>{
	private:
		ObjectHandler<ForceVolume> forcevolume_handler;
		std::vector<CollisionContact*> contacts;

		ContactResolver mContactResolver = ContactResolver(20);
		std::list<RigidObjectLink*> links;
	public:

		PhysicsHandler();

		void Update(double duration);
		void AddContact(RigidObject* r1, RigidObject* r2, float restitution, float depth);
		void AddLink(RigidObjectLink* link);

	protected:
		void GenerateContacts();
		void GetOverlap();
	};
}