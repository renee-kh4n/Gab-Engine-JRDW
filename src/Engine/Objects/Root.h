#pragma once

#include "Engine/Objects/Object.h"
#include "Engine/ObjectHandlers/ObjectHandler.h"

namespace gbe{
	class Root : public Object {
	private:
		std::list<Handler*> handlers;
	public:
		void RegisterHandler(Handler* handler);

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);

		template<typename T>
		T* GetHandler() {

		}
	};
}