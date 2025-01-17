#pragma once

namespace gbe {
	class PhysicsUpdate {
	public:
		virtual void InvokePhysicsUpdate(float deltatime) = 0;
	};
}