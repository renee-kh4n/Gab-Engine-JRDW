#pragma once

#include <unordered_map>
#include <string>
#include <list>

#include "InputAction.h"

namespace gde {
	namespace input {
		class InputSystem {
		private:
			std::list<InputAction> actions;
			std::unordered_map<std::string, std::list<InputAction>> player_actions;
		public:
			void RegisterAction(InputAction action) {
				actions.push_back(action);

				for (auto player_action_list : player_actions)
				{
					player_action_list.second.push_back(action);
				}
			}

			void AddPlayer(std::string name) {
				if (player_actions.find(name) == player_actions.end())
					return;

				player_actions.insert_or_assign(name, std::list<InputAction>(actions));
			}
		};
	}
}