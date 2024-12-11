#pragma once

#include "ControllerBase.h"

namespace gbe {
	class GenericController : public ControllerBase {
	private:
	public:
		void AddCustomer(InputCustomer_base*);
	};
}