#include "GDutil.h"

std::string LoadTxtFile(std::string path)
{
    std::fstream fsrc(path);
    std::stringstream fbuff;
    fbuff << fsrc.rdbuf();

    return fbuff.str();
}
