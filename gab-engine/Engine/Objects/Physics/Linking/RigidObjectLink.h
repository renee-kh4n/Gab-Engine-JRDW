#pragma once

#include "../RigidObject.h"
#include "../Collision/CollisionContact.h"
#include "../../../Datatypes/Vector3.h"

namespace gde {
	class RigidObjectLink {
	public:
		RigidObject* objects[2];
		virtual CollisionContact* GetContact() { return nullptr; }
	protected:
		float CurrentLength();
	};
}