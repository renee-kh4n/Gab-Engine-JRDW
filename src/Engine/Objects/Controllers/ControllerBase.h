#pragma once
#include <functional>
#include <vector>
#include "Engine/Objects/Object.h"
#include "Engine/Objects/Input/InputCustomer.h"

namespace gbe {

	class ControllerBase : public Object {
	protected:
		std::vector<InputCustomer_base*> inputreceivers;
	public:
		void ForEach_inputreceivers(std::function<void(InputCustomer_base*)>);
	};
}