#pragma once

#include "Objects/Physics/RigidObject.h"
#include "../Datatypes/Vector3.h"
#include "ObjectFunctions/Update.h"

namespace gbe {
	class Spinner : public RigidObject, public Update {
	public:

	private:
		Vector3 rotateSpeed = Vector3::zero;

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}