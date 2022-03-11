void ViewFiles()
{
    std::string directory = recv(1);
    struct dirent* entry;

    DIR* dir = opendir(directory.data());
    if (!dir) {
        send("invalid");
        return;

    } send("valid");

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;

        } data += entry->d_name; data += "\n";

    } closedir(dir); sendAll(data);
}