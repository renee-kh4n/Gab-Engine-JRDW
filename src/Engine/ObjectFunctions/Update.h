#pragma once

namespace gbe {
	class Update {
	public:
		virtual void InvokeUpdate(float deltatime) = 0;
	};
}