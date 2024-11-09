#include "InputPlayer.h"

gbe::InputPlayer::InputPlayer(std::string player_name)
{
	this->player_name = player_name;
}

std::string gbe::InputPlayer::get_player_name()
{
	return this->player_name;
}

void gbe::InputPlayer::OnEnterHierarchy(Object* other)
{
	Object::OnEnterHierarchy(other);

	controllers.TryAdd(other);
}

void gbe::InputPlayer::OnExitHierarchy(Object* other)
{
	Object::OnExitHierarchy(other);

	controllers.Remove(other);
}
