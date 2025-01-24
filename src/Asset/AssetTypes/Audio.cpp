#include "Audio.h"

gbe::asset::Audio::Audio(std::string path) : BaseAsset(path) {

}

void gbe::asset::Audio::Play(Vector3 local_position, float volume, int channel) {
	this->load_data.play(local_position, volume, channel);
}