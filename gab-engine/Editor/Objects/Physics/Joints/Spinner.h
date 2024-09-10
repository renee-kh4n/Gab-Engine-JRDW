#pragma once

#include "../RigidObject.h"
#include "Datatypes/Vector3.h"
#include "../../../ObjectFunctions/Update.h"

namespace gde {
	class Spinner : public RigidObject, public Update {
	public:

	private:
		Vector3 rotateSpeed = Vector3::zero;

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}