void ViewFiles(string directory)
{
    struct dirent* entry;
    string files;

    DIR* dir = opendir(directory.data());
    if (!dir) {
        sendall("invalid");
        return;
    
    } sendall("valid");

    try {
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
                
            } files += entry->d_name; files += "|";
        
        } closedir(dir); sendall(files);
    
    } catch (exception e) {
        return;
    }
}