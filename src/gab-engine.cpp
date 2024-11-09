// gab-engine.cpp : Defines the entry point for the application.
//

#include "gab-engine.h"

using namespace std;

int main(int argc, char* argv[])
{
	auto run_editor = true;

	if (run_editor) {
		auto editor = gbe::Editor();
		editor.Run();
	}
	else {
		auto engine = gbe::Engine();
		engine.Run();
	}
	return 0;
}
