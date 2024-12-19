#include "ControllerBase.h"
#include "Engine/Objects/Input/InputCustomer.h"

void gbe::ControllerBase::ForEach_inputreceivers(std::function<void(InputCustomer_base*)> function)
{
	for (auto receiver : this->inputreceivers)
	{
		function(receiver);
	}
}

