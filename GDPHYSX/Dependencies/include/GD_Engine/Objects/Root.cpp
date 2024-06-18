#include "Root.h"

void gde::Root::RegisterHandler(Handler* handler)
{
	this->handlers.push_back(handler);
}

void gde::Root::OnEnterHierarchy(Object* newChild)
{
	for (auto handler : this->handlers)
	{
		handler->TryAdd(newChild);
	}
}

void gde::Root::OnExitHierarchy(Object* newChild)
{
	for (auto handler : this->handlers)
	{
		handler->Remove(newChild);
	}
}
