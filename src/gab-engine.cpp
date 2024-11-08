// gab-engine.cpp : Defines the entry point for the application.
//

#include "gab-engine.h"

using namespace std;

int main(int argc, char* argv[])
{
	auto editor = gde::Editor();
	editor.Run();
	return 0;
}
