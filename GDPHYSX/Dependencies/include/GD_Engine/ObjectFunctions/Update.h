#pragma once

namespace gde {
	class Update {
	public:
		virtual void InvokeUpdate(float deltatime) = 0;
	};
}