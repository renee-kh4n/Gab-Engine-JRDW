#include "AudioPipeline.h"

gbe::audio::AudioPipeline* gbe::audio::AudioPipeline::Instance;

void gbe::audio::AudioPipeline::Init()
{
	AudioPipeline::Instance = this;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        throw("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    this->gMusic = Mix_LoadMUS("DefaultAssets/Audio/Can you Remastered.wav");
    if (this->gMusic == NULL)
    {
        throw("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    //Mix_PlayMusic(this->gMusic, -1);
}

gbe::audio::AudioPipeline::AudioPipeline()
{
    this->audioloader.AssignSelfAsLoader();
}

gbe::audio::AudioPipeline* gbe::audio::AudioPipeline::Get_Instance()
{
	return AudioPipeline::Instance;
}
