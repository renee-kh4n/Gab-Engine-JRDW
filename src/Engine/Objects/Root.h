#pragma once

#include "Engine/Objects/Object.h"
#include "Engine/ObjectHandlers/ObjectHandler.h"

namespace gbe{
	class Root : public Object {
	private:
		std::list<Handler*> handlers;
	public:
		~Root();
		void RegisterHandler(Handler* handler);

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);

		template<typename T>
		T* GetHandler() {
			T* toreturn = nullptr;

			for (auto handler : this->handlers)
			{
				toreturn = dynamic_cast<T*>(handler);

				if (toreturn != nullptr)
					break;
			}

			return toreturn;
		}
	};
}