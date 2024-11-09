#pragma once

#include "Object.h"
#include "../ObjectHandlers/ObjectHandlers.h"

namespace gbe{
	class Root : public Object {
	private:
		std::list<Handler*> handlers;
	public:
		void RegisterHandler(Handler* handler);

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);
	};
}