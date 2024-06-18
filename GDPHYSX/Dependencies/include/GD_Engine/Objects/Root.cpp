#include "Root.h"

using namespace gde;

void Root::RegisterHandler(Handler* handler)
{
	this->handlers.push_back(handler);
}

void Root::OnEnterHierarchy(Object* newChild)
{
	for (auto handler : this->handlers)
	{
		handler->TryAdd(newChild);
	}
}

void Root::OnExitHierarchy(Object* newChild)
{
	for (auto handler : this->handlers)
	{
		handler->Remove(newChild);
	}
}
