#include "BaseAsset.h"

std::string gbe::asset::BaseAsset::GetPath(int index)
{
	return this->paths[0];
}

bool gbe::asset::BaseAsset::Get_destroy_queued()
{
	return this->destroy_queued;
}
