#pragma once

#include "Objects/Physics/RigidObject.h"
#include "../Datatypes/Vector3.h"
#include "ObjectFunctions/Update.h"

namespace gbe {
	class Spinner : public RigidObject, public Update {
	public:
		Vector3 angularVel = Vector3::zero;
	private:

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}