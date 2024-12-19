#pragma once

#include <list>
#include <functional>
#include "Engine/Objects/Object.h"

namespace gbe {
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

		virtual void OnAdd(TValue*) {}
		virtual void OnRemove(TValue*) {}

		virtual void Remove(Object* object) {
			for (auto subhandler : this->subhandlers)
				subhandler->Remove(object);

			this->object_list.remove_if([this, object](TValue* tocheck) {
				Object* base_object = dynamic_cast<Object*>(tocheck);
				if (base_object == object) {
					OnRemove(tocheck);
					return true;
				}

				return false;
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
			OnAdd(typed_object);

			return true;
		}
	};
}