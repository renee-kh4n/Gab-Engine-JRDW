#include "Root.h"

void gde::Root::RegisterHandler(Handler* handler)
{
	this->handlers.push_back(handler);
}

void gde::Root::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	for (auto handler : this->handlers)
	{
		handler->TryAdd(newChild);
	}
}

void gde::Root::OnExitHierarchy(Object* newChild)
{
	Object::OnExitHierarchy(newChild);

	for (auto handler : this->handlers)
	{
		handler->Remove(newChild);
	}
}
