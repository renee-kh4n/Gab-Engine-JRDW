#include "Root.h"

void gbe::Root::RegisterHandler(Handler* handler)
{
	this->handlers.push_back(handler);
}

void gbe::Root::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	for (auto handler : this->handlers)
	{
		handler->TryAdd(newChild);
	}
}

void gbe::Root::OnExitHierarchy(Object* newChild)
{
	Object::OnExitHierarchy(newChild);

	for (auto handler : this->handlers)
	{
		handler->Remove(newChild);
	}
}
