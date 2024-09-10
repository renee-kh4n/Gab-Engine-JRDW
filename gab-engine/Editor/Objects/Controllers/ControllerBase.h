#pragma once

#include "../Object.h"
#include "../Input/InputCustomer.h"
#include <functional>

namespace gde {
	class ControllerBase : public Object {
	protected:
		std::vector<InputCustomer_base*> inputreceivers;
	public:
		void ForEach_inputreceivers(std::function<void(InputCustomer_base*)>);
	};
}