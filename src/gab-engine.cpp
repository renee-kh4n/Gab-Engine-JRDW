// gab-engine.cpp : Defines the entry point for the application.
//

#include "gab-engine.h"

int main(int argc, char* argv[])
{
    bool runengine = true;
    std::thread enginethread([]() {
        auto engine = new gbe::Engine();
        engine->Run();
    });

    bool runeditor = true;
    std::thread editorthread([&argc, &argv]() {
        auto editor = new gbe::Editor();
        editor->Run(argc, argv);
    });

    enginethread.join();
    editorthread.join();

    return 0;
}