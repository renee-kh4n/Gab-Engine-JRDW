#pragma once

#include "../Object.h"
#include "../Physics/RigidObject.h"
#include "InputCustomer.h"
#include "../../Input/Action/KeyPress.h"
#include "GLFW/glfw3.h"

namespace gde {
	class SpacebarHitter : public Object, public InputCustomer<KeyPress<GLFW_KEY_SPACE>> {
		RigidObject* toHit;
		Vector3 forceToHit;
		Vector3 positionToHit;
	public:
		SpacebarHitter(RigidObject* toHit, Vector3 forceToHit, Vector3 positionToHit = Vector3::zero);
		// Inherited via InputCustomer
		virtual void OnInput(KeyPress<GLFW_KEY_SPACE>* value, bool changed) override;
	};
}