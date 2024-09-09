#pragma once

#include <string>

#include "../Object.h"
#include "../Controllers/ControllerBase.h"
#include "../../ObjectHandlers/ObjectHandler.h"

namespace gde {
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