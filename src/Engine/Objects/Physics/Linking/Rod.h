#pragma once

#include "RigidObjectLink.h"

namespace gbe {
	class Rod : public RigidObjectLink {
	public:
		float length = 1;
		float restitution = 0;
		
		CollisionContact* GetContact() override;
	};
}