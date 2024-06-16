#pragma once

#include <unordered_map>
#include <string>
#include <list>
#include <functional>

#include "InputAction.h"
#include "ActionImplementation.h"

namespace gde {
	namespace input {
		class InputSystem {
		private:
			std::unordered_map<std::string, std::list<ActionImplementation_base*>> player_actions;
		public:
			void RegisterAction(std::string name, ActionImplementation_base* action) {
				if (player_actions.find(name) == player_actions.end())
					player_actions.insert_or_assign(name, std::list<ActionImplementation_base*>());

				player_actions[name].push_back(action);
			}
			
			void UpdateStates(std::function<void(std::string, InputAction*, bool)> callback) {
				for (auto player_action : player_actions)
					for (auto action_implementation : player_action.second) {
						bool changed = action_implementation->CheckStateChanged();

						callback(player_action.first, action_implementation->GetState(), changed);
					}
			}
		};
	}
}