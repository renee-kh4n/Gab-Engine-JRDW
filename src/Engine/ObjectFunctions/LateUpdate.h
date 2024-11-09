#pragma once

namespace gbe {
	class LateUpdate {
	public:
		virtual void InvokeLateUpdate(float deltatime) = 0;
	};
}