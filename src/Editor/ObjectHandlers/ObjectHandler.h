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

	template<class TValue>
	class ObjectHandler : public Handler {
	protected:
		std::list<Handler*> subhandlers;
	public:
		std::list<TValue*> object_list;

		virtual void Remove(Object* object) {
			for (auto subhandler : this->subhandlers)
				subhandler->Remove(object);

			this->object_list.remove_if([object](TValue* tocheck) {
				Object* base_object = dynamic_cast<Object*>(tocheck);
				return base_object == object;
			});
		}

		virtual bool TryAdd(Object* object) {
			for (auto subhandler : this->subhandlers)
				subhandler->TryAdd(object);

			TValue* typed_object = dynamic_cast<TValue*>(object);

			if (typed_object == nullptr)
				return false;

			for (auto existing : this->object_list)
				if (existing == typed_object)
					return false;

			object_list.push_back(typed_object);

			return true;
		}
	};
}