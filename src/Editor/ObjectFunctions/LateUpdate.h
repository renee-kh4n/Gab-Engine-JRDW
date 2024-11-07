#pragma once

namespace gde {
	class LateUpdate {
	public:
		virtual void InvokeLateUpdate(float deltatime) = 0;
	};
}