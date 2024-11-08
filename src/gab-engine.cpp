// gab-engine.cpp : Defines the entry point for the application.
//

#include "gab-engine.h"

using namespace std;

int main(int argc, char* argv[])
{
    /* Initialise SDL */
	SDL_Init(SDL_INIT_VIDEO);

	auto editor = gde::Editor();

	editor.Run();

	SDL_Quit();
	return 0;
}
