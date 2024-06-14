#pragma once

#include <list>
#include <functional>

#include "../Objects/Object.h"

namespace gde {
	class Handler {
	public:
		virtual void Remove(Object* object) = 0;
		virtual bool TryAdd(Object* object) = 0;
	};

	template<class T>
	class ObjectHandler : public Handler {
	protected:
		std::list<Handler*> subhandlers;
	public:
		std::list<T*> object_list;

		virtual void Remove(Object* object) {
			for (auto subhandler : this->subhandlers)
				subhandler->Remove(object);

			this->object_list.remove_if([object](T* tocheck) {return tocheck == object; });
		}

		virtual bool TryAdd(Object* object) {
			for (auto subhandler : this->subhandlers)
				subhandler->TryAdd(object);

			T* typed_object = dynamic_cast<T*>(object);

			if (typed_object == nullptr)
				return false;

			for (auto existing : this->object_list)
				if (existing == typed_object)
					return false;

			object_list.push_back(typed_object);
		}
	};
}