#pragma once

#include "../RigidObject.h"
#include "../../../Datatypes/Vector3.h"
#include "../../../ObjectFunctions/Update.h"

namespace gde {
	class ChainJoint : public Object, public Update {
	public:
		ChainJoint(float restLen)
			: restLen(restLen) {}

		RigidObject* to_rbody = nullptr;
	private:
		float restLen;

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}