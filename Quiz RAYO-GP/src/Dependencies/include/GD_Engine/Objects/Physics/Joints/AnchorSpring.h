#pragma once

#include "../RigidObject.h"
#include "../../../Datatypes/Vector3.h"
#include "../../../ObjectFunctions/Update.h"

namespace gde {
	class AnchorSpring : public Object, public Update {
	public:
		AnchorSpring(float springConst, float restLen)
			: springConst(springConst), restLen(restLen){}
		
		RigidObject* to_rbody = nullptr;
	private:
		RigidObject* this_rbody = nullptr;
		
		float springConst;
		float restLen;

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	public:
		virtual void SetParent(Object* newParent);
	};
}