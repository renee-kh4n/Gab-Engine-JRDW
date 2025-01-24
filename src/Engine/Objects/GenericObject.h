#pragma once

#include "Object.h"
#include "../ObjectFunctions/Update.h"
#include <functional>

namespace gbe {
	class GenericObject : public Object, public Update{
	private:
		std::function<void(GenericObject*, float)> onUpdate;
	public:
		GenericObject(std::function<void(GenericObject*, float)>);
		void InvokeUpdate(float delta) override;
	};
}