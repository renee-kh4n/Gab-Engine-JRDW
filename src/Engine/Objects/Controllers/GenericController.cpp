#include "GenericController.h"

void gbe::GenericController::AddCustomer(InputCustomer_base* customer)
{
	this->inputreceivers.push_back(customer);
}