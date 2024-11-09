#include "ControllerBase.h"

void gbe::ControllerBase::ForEach_inputreceivers(std::function<void(InputCustomer_base*)> function)
{
	for (auto receiver : this->inputreceivers)
	{
		function(receiver);
	}
}

