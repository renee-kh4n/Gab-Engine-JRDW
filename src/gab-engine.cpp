// gab-engine.cpp : Defines the entry point for the application.
//

#include "gab-engine.h"

int main(int argc, char* argv[])
{
    bool runengine = true;
    bool runeditor = false;

    std::thread editorthread;

    if (runeditor)
        editorthread = std::thread([&argc, &argv]() {
        auto editor = new gbe::Editor();
        editor->Run(argc, argv);
            });

    if (runengine)
    {
        auto engine = new gbe::Engine();
        engine->Run();
    }

    if (runeditor)
        editorthread.join();

    return 0;
}