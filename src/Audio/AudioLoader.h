#pragma once

#include "Asset/AssetLoading/AssetLoader.h"
#include "Asset/AssetTypes/Audio.h"
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace gbe {
	namespace audio {
		struct AudioData {
			Mix_Chunk* audio_chunk;
		};

		class AudioLoader : public asset::AssetLoader<asset::Audio, asset::data::AudioImportData, asset::data::AudioLoadData, AudioData> {
		private:
		protected:
			AudioData LoadAsset_(asset::Audio* asset, const asset::data::AudioImportData& importdata, asset::data::AudioLoadData* data) override;
			void UnLoadAsset_(asset::Audio* asset, const asset::data::AudioImportData& importdata, asset::data::AudioLoadData* data) override;
		};
	}
}