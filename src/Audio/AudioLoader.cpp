#include "AudioLoader.h"

bool gbe::audio::AudioLoader::LoadAsset_(asset::Audio* asset, const asset::data::AudioImportData& importdata, asset::data::AudioLoadData* data)
{
	auto directory = asset->Get_asset_directory() + importdata.filename;
	auto new_audio_data = Mix_LoadWAV(directory.c_str());
	if(new_audio_data == nullptr)
		return false;

	Mix_PlayChannel(-1, new_audio_data, 0);

	return true;
}

void gbe::audio::AudioLoader::UnLoadAsset_(asset::Audio* asset, const asset::data::AudioImportData& importdata, asset::data::AudioLoadData* data)
{

}
