#pragma once

#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "AudioLoader.h"

namespace gbe {
	namespace audio {
		class AudioPipeline {
		private:
			static AudioPipeline* Instance;

			//The music that will be played
			Mix_Music* gMusic = NULL;

			//The sound effects that will be used
			Mix_Chunk* gScratch = NULL;
			Mix_Chunk* gHigh = NULL;
			Mix_Chunk* gMedium = NULL;
			Mix_Chunk* gLow = NULL;

			//LOADERS
			AudioLoader audioloader;

		public:
			void Init();
			AudioPipeline();
			AudioPipeline* Get_Instance();
		};
	}
}