#include "AudioLoader.h"

gbe::audio::AudioData gbe::audio::AudioLoader::LoadAsset_(asset::Audio* asset, const asset::data::AudioImportData& importdata, asset::data::AudioLoadData* data)
{
	auto directory = asset->Get_asset_directory() + importdata.filename;
	auto new_audio_data = Mix_LoadWAV(directory.c_str());
	if (new_audio_data == nullptr)
		throw std::exception("Audio Failed to Load!");

	data->play = [new_audio_data](Vector3 pos, float volume, int channel) {
		Mix_PlayChannel(-1, new_audio_data, 0);
	};

	return AudioData{
		new_audio_data
	};
}

void gbe::audio::AudioLoader::UnLoadAsset_(asset::Audio* asset, const asset::data::AudioImportData& importdata, asset::data::AudioLoadData* data)
{

}
