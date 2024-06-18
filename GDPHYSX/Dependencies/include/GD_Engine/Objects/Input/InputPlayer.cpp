#include "InputPlayer.h"

namespace gde {
	InputPlayer::InputPlayer(std::string player_name)
	{
		this->player_name = player_name;
	}

	std::string InputPlayer::get_player_name()
	{
		return this->player_name;
	}

	void InputPlayer::OnEnterHierarchy(Object* other)
	{
		inputhandler.TryAdd(other);
	}

	void InputPlayer::OnExitHierarchy(Object* other)
	{
		inputhandler.Remove(other);
	}
}