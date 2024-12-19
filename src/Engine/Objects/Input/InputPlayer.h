#pragma once

#include <string>
#include "Engine/ObjectHandlers/ObjectHandler.h"
#include "Engine/Objects/Controllers/ControllerBase.h"
#include "Engine/Objects/Object.h"

namespace gbe {
	class InputPlayer : public Object {
	private:
		std::string player_name;
	public:
		InputPlayer(std::string player_name);
		std::string get_player_name();

		ObjectHandler<ControllerBase> controllers;

		virtual void OnEnterHierarchy(Object* other);
		virtual void OnExitHierarchy(Object* other);
	};
}