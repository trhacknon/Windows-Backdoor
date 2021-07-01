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