#pragma once


#include "../RigidObject.h"
#include "../Datatypes/Vector3.h"
#include "ObjectFunctions/Update.h"

namespace gde {
	class Bungee : public Object, public Update {
	public:
		Bungee(float springConst, float restLen)
			: springConst(springConst), restLen(restLen) {}

		RigidObject* to_rbody = nullptr;
	private:
		float springConst;
		float restLen;

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}