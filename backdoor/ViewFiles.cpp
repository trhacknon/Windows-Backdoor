#include "utilities.h"

extern std::string data;

void ViewFiles()
{
    send("success");
    std::string directory = recv();
    struct dirent* entry;

    DIR* dir = opendir(directory.data());
    if (!dir) {
        send("invalid");
        return;

    } send("valid");

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;

        } data += entry->d_name; data += "|";

    } closedir(dir); sendAll(data);
}