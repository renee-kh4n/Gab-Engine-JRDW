// gab-engine.cpp : Defines the entry point for the application.
//

#include "gab-engine.h"

int main(int argc, char* argv[])
{
    auto engine = new gbe::Engine();
    engine->Run();

    return 0;
}