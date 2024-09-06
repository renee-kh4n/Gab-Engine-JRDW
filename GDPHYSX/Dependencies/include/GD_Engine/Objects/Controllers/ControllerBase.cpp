#include "ControllerBase.h"

void gde::ControllerBase::ForEach_inputreceivers(std::function<void(InputCustomer_base*)> function)
{
	for (auto receiver : this->inputreceivers)
	{
		function(receiver);
	}
}

